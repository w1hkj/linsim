//======================================================================
//  linsim; main
//    Author: Dave Freese, W1HKJ <w1hkj@w1hkj.com>
//
//  based on the program PathSim by Moe Wheatley AE4JY
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the
//
//  Free Software Foundation, Inc.
//  51 Franklin Street, Fifth Floor
//  Boston, MA  02110-1301 USA.
//
// ---------------------------------------------------------------------

#include "main.h"
#include <string>
#include <vector>

#include "sound.h"

#include <fstream>
#include <ostream>

#include <cstring>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/x.H>
#include <FL/Fl_Help_Dialog.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>

#include "config.h"
#include "sound.h"
#include "util.h"
#include "configure.h"
#include "xmlrpc_client.h"
#include "linsim_ui.h"

#ifdef WIN32
#	include "linsimrc.h"
#	include "compat.h"
#	define dirent fl_dirent_no_thanks
#endif

#include <FL/filename.H>

#ifdef __MINGW32__
#	if LINSIM_FLTK_API_MAJOR == 1 && LINSIM_FLTK_API_MINOR < 3
#		undef dirent
#		include <dirent.h>
#	endif
#	ifndef IDI_ICON
#		define IDI_ICON 101
#	endif
#else
#	include <dirent.h>
#endif

using namespace std;

#include <FL/x.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_File_Chooser.H>

void fatal_error(string sz_error);
void process_run_simulation(void);
void process_batch_items(void);
void process_AWGN_series(void);
void delay_start_up(void *);
void process_modem_series(int run_type);
inline bool modem_name_by_index(std::string &modem, int index);
bool fldigi_record_audio(std::string modem);
bool fldigi_play_audio(void);
void generate_output(bool update_sim_data);

std::string BaseDir = "";
std::string HomeDir = "";

#define KNAME "linsim"
#if !defined(__WIN32__) && !defined(__APPLE__)

#	include "linsim_icon.cxx"

Pixmap	linsim_icon_pixmap;

void make_pixmap(Pixmap *xpm, const char **data)
{
	Fl_Window w(0,0, PACKAGE_NAME);
	w.xclass(PACKAGE_NAME);
	w.show();
	w.make_current();
	Fl_Pixmap icon(data);
	int maxd = (icon.w() > icon.h()) ? icon.w() : icon.h();
	*xpm = fl_create_offscreen(maxd, maxd);
	fl_begin_offscreen(*xpm);
	fl_color(FL_BACKGROUND_COLOR);
	fl_rectf(0, 0, maxd, maxd);
	icon.draw(maxd - icon.w(), maxd - icon.h());
	fl_end_offscreen();
}

#endif

csvdb simulations;
SIM sim_test;
_vals sim_vals;
std::string fname_in;
std::string fname_out;
std::string batch_dir;

// Modem Series
#define DEFAULT_FNAME_IN  "input"
#define DEFAULT_FNAME_OUT "output"
#define SOUND_FILE_EXT    ".wav"

enum {
	BATCH_RUN = 1,
	AWGN_RUN,
	PROCESS_RUN
};

vector<_vals> fname_out_vals;
vector<std::string> fname_out_sim_name;
vector<std::string> fname_out_list;

std::string modem_list;
std::string start_modem_name;
std::string end_modem_name;
std::string modem_series_tx_string;
std::string current_modem;
std::string input_filename;
std::string output_filename;

extern bool fldigi_online;
const char limited_char_list[] = "aeiouy012345";
bool  modem_series_flag = false;
int   character_history[256];
int   char_tx_repeat_count      = 0;
float tx_duration_minimum       = 2.0;
bool  modem_list_populated      = false;
int   start_modem_index_integer = 0;
int   end_modem_index_integer   = 0;
float modem_sample_rate         = 0;
float file_sample_rate          = 0;

// End Modem Series

Fl_Double_Window *simulator_selector = (Fl_Double_Window *)0;
Fl_Double_Window *batch_process_selector = (Fl_Double_Window *)0;
Fl_Double_Window *AWGN_process_dialog = (Fl_Double_Window *)0;
Fl_Double_Window *select_output_dialog = (Fl_Double_Window *)0;
Fl_Double_Window *modem_series_dialog = (Fl_Double_Window *)0;
Fl_Double_Window *ip_address_dialog = (Fl_Double_Window *)0;
Fl_Double_Window *models_dialog = (Fl_Double_Window *)0;
Fl_Double_Window *tx_duration_dialog = (Fl_Double_Window *)0;

char title[50];
char progdir[80];

int handle (int e) {
	if (Fl::focus() == linsim_window)
		return (e == FL_SHORTCUT);
	return 0;
}

void visit_URL(void* arg)
{
	const char* url = reinterpret_cast<const char *>(arg);
#ifndef __WOE32__
	const char* browsers[] = {
#  ifdef __APPLE__
		getenv("FLDIGI_BROWSER"), // valid for any OS - set by user
		"open"                    // OS X
#  else
		"fl-xdg-open",            // Puppy Linux
		"xdg-open",               // other Unix-Linux distros
		getenv("FLDIGI_BROWSER"), // force use of spec'd browser
		getenv("BROWSER"),        // most Linux distributions
		"sensible-browser",
		"firefox",
		"mozilla"                 // must be something out there!
#  endif
	};
	switch (fork()) {
		case 0:
#  ifndef NDEBUG
			unsetenv("MALLOC_CHECK_");
			unsetenv("MALLOC_PERTURB_");
#  endif
			for (size_t i = 0; i < sizeof(browsers)/sizeof(browsers[0]); i++)
				if (browsers[i])
					execlp(browsers[i], browsers[i], url, (char*)0);
			fatal_error("Could not find a web browser");
			exit(EXIT_FAILURE);
		case -1:
			fl_alert(
					 "Could not run a web browser:\n%s\n\n"
					 "Open this URL manually:\n%s",
					 strerror(errno), url);
	}
#else
	// gurgle... gurgle... HOWL
	// "The return value is cast as an HINSTANCE for backward
	// compatibility with 16-bit Windows applications. It is
	// not a true HINSTANCE, however. The only thing that can
	// be done with the returned HINSTANCE is to cast it to an
	// int and compare it with the value 32 or one of the error
	// codes below." (Error codes omitted to preserve sanity).
	if ((int)ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL) <= 32)
		fl_alert("Could not open url:\n%s\n", url);
#endif
}

pthread_t *comp_thread = 0;
pthread_mutex_t mutex_comp = PTHREAD_MUTEX_INITIALIZER;
bool exit_comp_loop = false;
bool op_abort = false;

enum {IDLE, RUN, BATCH, AWGN_SERIES};
int  simulation = IDLE;

void reset_buttons(void *)
{
	btn_abort->hide();
	btn_test->show();
}

void * comp_loop(void *d)
{
	exit_comp_loop = false;

	while(!exit_comp_loop) {
		try {
			switch (simulation) {
				case RUN :
					if(modem_series_flag)
						process_modem_series(PROCESS_RUN);
					else
						process_run_simulation();

					simulation = IDLE;
					Fl::awake(reset_buttons);
					Fl::awake(clear_main_dialog);
					op_abort = false;
					break;

				case BATCH :
					if(modem_series_flag)
						process_modem_series(BATCH_RUN);
					else
						process_batch_items();

					simulation = IDLE;
					Fl::awake(reset_buttons);
					Fl::awake(clear_main_dialog);
					op_abort = false;
					break;

				case AWGN_SERIES :
					if(modem_series_flag)
						process_modem_series(AWGN_RUN);
					else
						process_AWGN_series();

					simulation = IDLE;
					Fl::awake(reset_buttons);
					Fl::awake(clear_main_dialog);
					op_abort = false;
					break;
				case IDLE :
				default : ;
			}
		} catch (const std::string errmsg) {
			printf("fatal error: %s\n", errmsg.c_str());
			exit(0);
		}
		MilliSleep(250);
	}
	return NULL;
}

void start_computation_thread()
{
	comp_thread = new pthread_t;
	if (pthread_create(comp_thread, NULL, comp_loop, NULL)) {
		fatal_error("pthread_create");
		exit(EXIT_FAILURE);
	}
}

void clean_exit()
{
	btn_abort->hide();
	btn_test->show();
	op_abort = true;

	simulations.save();
	progStatus.saveLastState();

	if (simulator_selector)     simulator_selector->hide();;
	if (batch_process_selector) batch_process_selector->hide();
	if (AWGN_process_dialog)    AWGN_process_dialog->hide();
	if (select_output_dialog)   select_output_dialog->hide();
	if (modem_series_dialog)    modem_series_dialog->hide();
	if (ip_address_dialog)      ip_address_dialog->hide();
	if (tx_duration_dialog)     tx_duration_dialog->hide();

	terminate_xmlrpc();

	exit_comp_loop = true;
	pthread_join(*comp_thread, 0);

	linsim_window->hide();
}

void exit_main(Fl_Widget *w)
{
	if (Fl::event_key() == FL_Escape)
		return;
	clean_exit();
}

// Show an error dialog and print to cerr if available.
// On win32 Fl::fatal displays its own error window.
void fatal_error(string sz_error)
{
	string s = "Fatal error!\n";
	s.append(sz_error).append("\n").append(strerror(errno));

	// Win32 will display a MessageBox error message
#if !defined(__WOE32__)
	fl_message_font(FL_HELVETICA, FL_NORMAL_SIZE);
	fl_alert("%s", s.c_str());
#endif
	Fl::fatal(s.c_str());
}

int main(int argc, char **argv)
{
	int arg_idx;

	Fl::args(argc, argv, arg_idx, parse_args);

	Fl::add_handler (handle);

	linsim_window          = make_linsim_window();
	simulator_selector     = make_selector_dialog();
	batch_process_selector = make_batch_selector_dialog();
	AWGN_process_dialog    = make_AWGNseries_dialog();
	select_output_dialog   = make_folder_dialog();
	modem_series_dialog    = make_ModemSeries_dialog();

	sprintf (title, "linsim %s", linsim_VERSION);
	linsim_window->label(title);
	linsim_window->callback(exit_main);

#ifdef __WOE32__
	char dirbuf[FL_PATH_MAX + 1];
	fl_filename_expand(dirbuf, sizeof(dirbuf) -1, "$USERPROFILE/");
	BaseDir.assign(dirbuf);
	HomeDir.assign(dirbuf).append("linsim.files/");
#else
	char dirbuf[FL_PATH_MAX + 1];
	fl_filename_expand(dirbuf, sizeof(dirbuf) -1, "$HOME/");
	BaseDir.assign(dirbuf);
	HomeDir.assign(dirbuf).append(".linsim/");
#endif

	if (mkdir(HomeDir.c_str(), 0777) == -1 && errno != EEXIST) {
		string s = "Could not create linsim directory ";
		s.append(HomeDir);
		fatal_error(s);
	}

	Fl::lock();
	Fl::scheme("gtk+");

#ifdef WIN32
	linsim_window->icon((char*)LoadIcon(fl_display, MAKEINTRESOURCE(IDI_ICON)));
	linsim_window->show(argc, argv);
#elif !defined(__APPLE__)
	make_pixmap(&linsim_icon_pixmap, linsim_icon);
	linsim_window->icon((char *)linsim_icon_pixmap);
	linsim_window->show(argc, argv);
#else
	linsim_window->show(argc, argv);
#endif

	string csv_fname;
	csv_fname.assign(HomeDir).append("linsim.simulations.csv");
	simulations.filename(csv_fname);
	txt_simulations_filename->value("linsim.simulations.csv");
	simulations.load();

	progStatus.loadLastState();

	start_computation_thread();
	start_xmlrpc();

	Fl::add_timeout(1.0, delay_start_up);

	return Fl::run();
}

int parse_args(int argc, char **argv, int& idx)
{
	if (strcasecmp("--help", argv[idx]) == 0) {
		printf("Usage: \n\
			   --help this help text\n\
			   --version\n");
		exit(0);
	}
	if (strcasecmp("--version", argv[idx]) == 0) {
		printf("Version: "VERSION"\n");
		exit (0);
	}
	return 0;
}

void delay_start_up(void *)
{
	progStatus.initLastState();
}

void update_sim_vals()
{
	sim_vals.p0.active = p0_on->value();
	sim_vals.p0.spread = atof(inp_spread0->value());
	sim_vals.p0.offset = atof(inp_offset0->value());

	sim_vals.p1.active = p1_on->value();
	sim_vals.p1.spread = atof(inp_spread1->value());
	sim_vals.p1.offset = atof(inp_offset1->value());

	sim_vals.p2.active = p2_on->value();
	sim_vals.p2.spread = atof(inp_spread2->value());
	sim_vals.p2.offset = atof(inp_offset2->value());

	sim_vals.d.delay1 = atof(inp_delay1->value());
	sim_vals.d.delay2 = atof(inp_delay2->value());

	sim_vals.AWGN_on  = inp_AWGN_on->value();
	sim_vals.snrdb    = atof(inp_AWGN_rms->value());
}

void set_output_sr()
{
	if (mnu_sr_as_input->value() == 4)
		outfile_samplerate = inpfile_samplerate;
	else if (mnu_sr_8000->value() == 4)
		outfile_samplerate = 8000;
	else if (mnu_sr_11025->value() == 4)
		outfile_samplerate = 11025;
	else if (mnu_sr_16000->value() == 4)
		outfile_samplerate = 16000;
	else if (mnu_sr_22050->value() == 4)
		outfile_samplerate = 22050;
	else if (mnu_sr_24000->value() == 4)
		outfile_samplerate = 24000;
	else if (mnu_sr_44100->value() == 4)
		outfile_samplerate = 44100;
	else if (mnu_sr_48000->value() == 4)
		outfile_samplerate = 48000;
}

std::string str_progress = "";
float f_progress = 0.0;
float f_progress_max = 1.0;
float f_progress_min = 0;
bool progress_blink_flag = false;
float blink_duration = 1.0;

void set_progress(void *data)
{
	progress->label(str_progress.c_str());
	progress->redraw_label();
	Fl::flush();
}

void update_progress(void *data)
{
	progress->value(f_progress);
	progress->redraw();
	Fl::flush();
}

void update_progress_label(void *message)
{
	if(message) {
		str_progress.assign((char *) message);
		set_progress((void *)0);
	}
}

void progress_blink_timer(void *v)
{
	if(progress_blink_flag) {
		if(f_progress < f_progress_max)
			f_progress = f_progress_max;
		else
			f_progress = f_progress_min;

		Fl::add_timeout(blink_duration, progress_blink_timer);
	} else {
		update_progress_label((void *)"");
		f_progress = f_progress_min;
	}

	update_progress((void *) 0);
}

void start_blink_progress(void * message)
{
	progress_blink_flag = true;

	if(message) {
		str_progress.assign((char *) message);
		set_progress((void *)0);
	}

	f_progress_max = progress->maximum();
	f_progress_min = progress->minimum();

	f_progress = f_progress_min;

	update_progress((void *)0);

	Fl::flush();

	Fl::add_timeout(blink_duration, progress_blink_timer);
}

void stop_blink_progress(void *v)
{
	if(progress_blink_flag)
		progress_blink_flag = false;
}

size_t buffs_read;

// CALLED ONLY FROM THE COMP THREAD
void analyze_input()
{
	if (fname_in.empty()) return;
	string dir = fname_in;
	size_t p = dir.find(fl_filename_name(dir.c_str()));
	if (p != string::npos) dir.erase(p);

	sim_test.sound_in.open(fname_in, SoundFile::READ);
	int read_unit_count = MAX_FRAME_COUNT * sim_test.sound_in.io_channels();
	Allocate<double> B;
	double *buffer = B.allocate(read_unit_count);
	if(!buffer) return;

	size_t r = 0;
	sim_test.signal_rms = 0.0;

	str_progress = "Analyzing input";
	Fl::awake(set_progress);
	if (op_abort) return;

	sim_test.signal_rms = 0.0;
	sim_test.num_buffs = 0;

	buffs_read = 0;
	sim_test.signal_peak = 0;

	size_t num_buffs = sim_test.sound_in.size() / MAX_FRAME_COUNT;

	while ((r = sim_test.sound_in.read(buffer, read_unit_count)) > 0) {
		sim_test.measure_rms(buffer, r);
		// sim_test.measure_rms(buffer, MAX_FRAME_COUNT);
		buffs_read++;
		memset(buffer, 0, read_unit_count * sizeof(double));
		f_progress = 1.0 * buffs_read / num_buffs;
		Fl::awake(update_progress);
		if (op_abort) break;
	}
	sim_test.signal_rms = sqrtf(sim_test.signal_rms / sim_test.num_buffs);

	if (sim_test.signal_rms < 1e-6) sim_test.signal_rms = 1e-6;

	sim_test.sound_in.close();
}

extern int linsim_samplerate;

// CALLED ONLY FROM THE COMP THREAD
void generate_output(bool update_sim_data)
{
	if (fname_out.empty()) return;

	Allocate<double> B;
	double *buffer = B.allocate(MAX_FRAME_COUNT);
	if(!buffer) return;

	if(update_sim_data)
		update_sim_vals();

	if(modem_series_flag)
		fname_out_vals.push_back(sim_vals);

	sim_test.AWGN(sim_vals.AWGN_on);
	sim_test.SetsnrValue(sim_vals.snrdb);

	init_BPFir(linsim_samplerate);
	sim_test.init(linsim_samplerate, sim_vals.p0, sim_vals.p1, sim_vals.p2, sim_vals.d);

	set_output_sr();

	sim_test.noise_rms = (sim_test.signal_rms / sim_test.snr) / 5;

	//std::cout << "peak  " << sim_test.signal_peak << "\n";
	//std::cout << "rms   " << sim_test.signal_rms << "\n";
	//std::cout << "snr   " << sim_test.snr << "\n";
	//std::cout << "noise " << sim_test.noise_rms << "\n";

	sim_test.sound_in.open(fname_in, SoundFile::READ);

	size_t pq = 0;
	size_t r_frames = 0;
	size_t step = sim_test.sound_in.io_channels();
	size_t i = 0;
	size_t units = 0;
	double maxsig = 1e-6;

	f_progress = 0.0;
	Fl::awake(update_progress);
	str_progress = "Running Simulation";
	Fl::awake(set_progress);

	while ((r_frames = sim_test.sound_in.read(buffer, MAX_FRAME_COUNT)) > 0) {
		units = r_frames * step;
		f_progress = 1.0 * (++pq) / buffs_read;
		Fl::awake(update_progress);
		if (op_abort) break;
		sim_test.Process(buffer, MAX_FRAME_COUNT);
		for (i = 0; i < units; i += step)
			if (fabs(buffer[i]) > maxsig) maxsig = fabs(buffer[i]);
		memset(buffer, 0, MAX_FRAME_COUNT * sizeof(double));
	}
	if (op_abort) return;

	if (sim_test.sound_out.open(fname_out, SoundFile::WRITE) != 0) {
		throw "Could not open sound file for WRITE";
	}

	f_progress = 0.0;
	Fl::awake(update_progress);
	str_progress = "Writing Output";
	Fl::awake(set_progress);

	sim_test.sound_in.rewind();
	pq = 0;
	memset(buffer, 0, MAX_FRAME_COUNT * sizeof(double));
	while ((r_frames = sim_test.sound_in.read(buffer, MAX_FRAME_COUNT)) > 0) {
		units = r_frames * step;
		f_progress = 1.0 * (++pq) / buffs_read;
		Fl::awake(update_progress);
		if (op_abort) break;
		sim_test.Process(buffer, MAX_FRAME_COUNT);
		for (i = 0; i < units; i += step)
			buffer[i] *= (0.9 / maxsig);
		sim_test.sound_out.write(buffer, r_frames * step);
		memset(buffer, 0, MAX_FRAME_COUNT * sizeof(double));
	}

	sim_test.sound_out.close();
	sim_test.sound_in.close();

	str_progress = "";
	Fl::awake(set_progress);
	f_progress = 0.0;
	Fl::awake(update_progress);
}

// CALLED ONLY FROM THE COMP THREAD
void process_run_simulation(void)
{
	btn_test->hide();
	btn_abort->show();

	std::string basename;

	if(modem_series_flag) {
		basename = fname_in;
	} else if (btn_same_as_input_file->value()) {
		fname_in = input_filename;
		basename = fname_in;
	} else {
		string name = fl_filename_name(input_filename.c_str());
		basename = finp_output_wav_folder->value();
		basename.append(name);
	}

	if (basename.empty())
		return;

	size_t p = basename.find(".wav");
	if (p != string::npos) basename.erase(p);

	basename.append(".proc_run.wav");
	fname_out.assign(basename);

	if(modem_series_flag) {
		fname_out_list.push_back(fname_out);
	}

	analyze_input();
	generate_output(true);
}

void run_simulation()
{
	simulation = RUN;
}

void populate_simulation_selector()
{
	tbl_simulations->clear();
	simulations.sort();
	for (int n = 0; n < simulations.numrecs(); n++)
		tbl_simulations->add(simulations.dbrecs[n].title.c_str());
	tbl_simulations->redraw();
}

void cancel_selection()
{
	btn_delete_selection->deactivate();
	btn_update_selection->deactivate();
	simulator_selector->hide();
}

void delete_selection(int n)
{
	btn_delete_selection->deactivate();
	btn_update_selection->deactivate();
	if (!n) return;
	simulations.erase(n-1);
	populate_simulation_selector();
}

void update_selection(int n)
{
	btn_delete_selection->deactivate();
	btn_update_selection->deactivate();

	if (!n) return;
	n--;
	simulations.dbrecs[n].title = txt_simulation->value();


	simulations.dbrecs[n].awgn = inp_AWGN_on->value() ? "1" : "0";
	simulations.dbrecs[n].sn = inp_AWGN_rms->value();

	simulations.dbrecs[n].p0 = p0_on->value() ? "1" : "0";
	simulations.dbrecs[n].spread_0 = inp_spread0->value();
	simulations.dbrecs[n].offset_0 = inp_offset0->value();

	simulations.dbrecs[n].p1 = p1_on->value() ? "1" : "0";
	simulations.dbrecs[n].delay_1 = inp_delay1->value();
	simulations.dbrecs[n].spread_1 = inp_spread1->value();
	simulations.dbrecs[n].offset_1 = inp_offset1->value();

	simulations.dbrecs[n].p2 = p2_on->value() ? "1" : "0";
	simulations.dbrecs[n].delay_2 = inp_delay2->value();
	simulations.dbrecs[n].spread_2 = inp_spread2->value();
	simulations.dbrecs[n].offset_2 = inp_offset2->value();

	populate_simulation_selector();
}

void add_simulation()
{
	if (!simulator_selector) simulator_selector = make_selector_dialog();

	csvRecord rec;

	rec.title    = txt_simulation->value();
	rec.awgn     = inp_AWGN_on->value() ? "1" : "0";
	rec.sn       = inp_AWGN_rms->value();
	rec.p0       = p0_on->value() ? "1" : "0";
	rec.spread_0 = inp_spread0->value();
	rec.offset_0 = inp_offset0->value();
	rec.p1       = p1_on->value() ? "1" : "0";
	rec.delay_1  = inp_delay1->value();
	rec.spread_1 = inp_spread1->value();
	rec.offset_1 = inp_offset1->value();
	rec.p2       = p2_on->value() ? "1" : "0";
	rec.delay_2  = inp_delay2->value();
	rec.spread_2 = inp_spread2->value();
	rec.offset_2 = inp_offset2->value();

	simulations.add(rec);
	populate_simulation_selector();
}

void tbl_simulations_selected()
{
	btn_delete_selection->deactivate();
	btn_update_selection->deactivate();
}

static std::string ofname = "";
void show_ofname(void *)
{
	txt_output_file->value(ofname.c_str());
}

static void show_what_simulation(void *d)
{
	int n = reinterpret_cast<int>(d);
	txt_simulation->value(simulations.dbrecs[n].title.c_str());
}

static void show_p0(void *d)
{
	int n = reinterpret_cast<int>(d);
	bool b = simulations.dbrecs[n].p0 == "1";
	p0_on->value(b);
	inp_spread0->value(simulations.dbrecs[n].spread_0.c_str());
	inp_offset0->value(simulations.dbrecs[n].offset_0.c_str());
}

static void show_p1(void *d)
{
	int n = reinterpret_cast<int>(d);
	bool b = simulations.dbrecs[n].p1 == "1";
	p1_on->value(b);
	inp_delay1->value(simulations.dbrecs[n].delay_1.c_str());
	inp_spread1->value(simulations.dbrecs[n].spread_1.c_str());
	inp_offset1->value(simulations.dbrecs[n].offset_1.c_str());
}

static void show_p2(void *d)
{
	int n = reinterpret_cast<int>(d);
	bool b = simulations.dbrecs[n].p2 == "1";
	p2_on->value(b);
	inp_delay2->value(simulations.dbrecs[n].delay_2.c_str());
	inp_spread2->value(simulations.dbrecs[n].spread_2.c_str());
	inp_offset2->value(simulations.dbrecs[n].offset_2.c_str());
}

static void show_simulations(void *d)
{
	int n = reinterpret_cast<int>(d);
	bool b = simulations.dbrecs[n].awgn == "1";
	inp_AWGN_on->value(b);
	inp_AWGN_rms->value(simulations.dbrecs[n].sn.c_str());
}

void select_entry(int n)
{
	//	simulator_selector->hide();
	if (!n) return;
	n--;

	//	simulations.printrec(n);

	txt_simulation->value(simulations.dbrecs[n].title.c_str());

	bool b = simulations.dbrecs[n].p0 == "1";
	p0_on->value(b);
	inp_spread0->value(simulations.dbrecs[n].spread_0.c_str());
	inp_offset0->value(simulations.dbrecs[n].offset_0.c_str());

	b = simulations.dbrecs[n].p1 == "1";
	p1_on->value(b);
	inp_delay1->value(simulations.dbrecs[n].delay_1.c_str());
	inp_spread1->value(simulations.dbrecs[n].spread_1.c_str());
	inp_offset1->value(simulations.dbrecs[n].offset_1.c_str());

	b = simulations.dbrecs[n].p2 == "1";
	p2_on->value(b);
	inp_delay2->value(simulations.dbrecs[n].delay_2.c_str());
	inp_spread2->value(simulations.dbrecs[n].spread_2.c_str());
	inp_offset2->value(simulations.dbrecs[n].offset_2.c_str());

	b = simulations.dbrecs[n].awgn == "1";
	inp_AWGN_on->value(b);
	inp_AWGN_rms->value(simulations.dbrecs[n].sn.c_str());

	btn_delete_selection->activate();
	btn_update_selection->activate();
}

void update_sim_variables(int n)
{
	//	simulator_selector->hide();
	if (!n) return;
	n--;

	sim_vals.AWGN_on = simulations.dbrecs[n].awgn == "1";
	sim_vals.snrdb   = atof(simulations.dbrecs[n].sn.c_str());

	sim_vals.p0.active = simulations.dbrecs[n].p0 == "1";
	sim_vals.p0.offset = atof(simulations.dbrecs[n].offset_0.c_str());
	sim_vals.p0.spread = atof(simulations.dbrecs[n].spread_0.c_str());

	sim_vals.p1.active = simulations.dbrecs[n].p1 == "1";;
	sim_vals.p1.offset = atof(simulations.dbrecs[n].offset_1.c_str());
	sim_vals.p1.spread = atof(simulations.dbrecs[n].spread_1.c_str());

	sim_vals.p2.active = simulations.dbrecs[n].p2 == "1";
	sim_vals.p2.offset = atof(simulations.dbrecs[n].offset_2.c_str());
	sim_vals.p2.spread = atof(simulations.dbrecs[n].spread_2.c_str());

	sim_vals.d.delay1 = atof(simulations.dbrecs[n].delay_1.c_str());
	sim_vals.d.delay2 = atof(simulations.dbrecs[n].delay_2.c_str());
}

void select_simulation()
{
	if (!simulator_selector) simulator_selector = make_selector_dialog();
	populate_simulation_selector();
	simulator_selector->show();
}

void populate_batch_selector()
{
	tbl_batch_simulations->clear();
	if (simulations.numrecs() == 0) return;
	simulations.sort();
	for (int n = 0; n < simulations.numrecs(); n++)
		tbl_batch_simulations->add(simulations.dbrecs[n].title.c_str());
	tbl_batch_simulations->redraw();
}

void clear_all_simulation()
{
	tbl_batch_simulations->check_none();
}

void select_all_simulations()
{
	tbl_batch_simulations->check_all();
}

void cancel_batch_process()
{
	batch_process_selector->hide();
}

void clear_main_dialog(void *data)
{
	txt_simulation->value("");

	p0_on->value(0);
	inp_spread0->value("");
	inp_offset0->value("");
	p1_on->value(0);
	inp_delay1->value("");
	inp_spread1->value("");
	inp_offset1->value("");
	p2_on->value(0);
	inp_delay2->value("");
	inp_spread2->value("");
	inp_offset2->value("");
	inp_AWGN_on->value(0);
	inp_AWGN_rms->value("");

	txt_output_file->value("");
	txt_input_file->value("");
	fname_in = fname_out = "";

	progress->label("");
	progress->redraw_label();

	progress->value(0);
	progress->redraw();
	Fl::flush();

	lbl_batch->hide();

}

void clear_AWGN(void *d)
{
	lbl_batch->show();
	p0_on->value(0);
	inp_spread0->value(0);
	inp_offset0->value(0);
	p1_on->value(0);
	inp_delay1->value(0);
	inp_spread1->value(0);
	inp_offset1->value(0);
	p2_on->value(0);
	inp_delay2->value(0);
	inp_spread2->value(0);
	inp_offset2->value(0);
	inp_AWGN_on->value(1);
}


// CALLED ONLY FROM THE COMP THREAD
void process_batch_items(void)
{
	std::string basename;
	std::string simname;
	std::string temp;

	if(modem_series_flag) {
		basename = fname_in;
	} else if (btn_same_as_input_file->value()) {
		fname_in = input_filename;
		basename = fname_in;
	} else {
		string name = fl_filename_name(input_filename.c_str());
		basename = finp_output_wav_folder->value();
		basename.append(name);
	}

	if (basename.empty())
		return;

	size_t p = basename.find(".wav");
	if (p != string::npos) basename.erase(p);
	basename.append(".");

	analyze_input();

	for (int n = 0; n < tbl_batch_simulations->nitems(); n++) {
		if (tbl_batch_simulations->checked(n+1)) {
			fname_out.assign(basename);
			simname.assign(tbl_batch_simulations->text(n+1));

			while ((p = simname.find("(")) != string::npos) simname.erase(p,1);
			while ((p = simname.find(")")) != string::npos) simname.erase(p,1);
			while ((p = simname.find("/")) != string::npos) simname[p] = '_';
			while ((p = simname.find("-")) != string::npos) simname[p] = '_';
			while ((p = simname.find(" ")) != string::npos) simname[p] = '_';

			fname_out.append(simname).append(".wav");
			ofname = fname_out;

			Fl::awake(show_ofname);
			Fl::awake(show_what_simulation, reinterpret_cast<void *>(n));
			Fl::awake(show_p0, reinterpret_cast<void *>(n));
			Fl::awake(show_p1, reinterpret_cast<void *>(n));
			Fl::awake(show_p2, reinterpret_cast<void *>(n));
			Fl::awake(show_simulations, reinterpret_cast<void *>(n));

			if (op_abort) break;

			update_sim_variables(n+1);

			if(modem_series_flag) {
				fname_out_list.push_back(fname_out);
				fname_out_sim_name.push_back(simname);
			}

			generate_output(false);
		}
	}
	Fl::awake(clear_main_dialog);
}

void batch_process_items(void)
{
	batch_process_selector->hide();
	simulation = BATCH;
}

void open_batch_process_dialog()
{
	if (!batch_process_selector) make_batch_selector_dialog();
	if (simulations.numrecs() == 0) return;
	populate_batch_selector();
	batch_process_selector->show();
}

void load_simulation_set()
{
	char *picked = fl_file_chooser (
									"Load Simulation Set",
									"*.csv",
									simulations.filename().c_str(), 1);
	if (!picked) return;
	simulations.filename(picked);
	if (simulations.load() == 0)
		txt_simulations_filename->value(fl_filename_name(simulations.filename().c_str()));
}

void save_simulation_set()
{
	simulations.save();
}

void save_simulation_set_as()
{
	char *picked = fl_file_chooser (
									"Save Simulation Set",
									"*.csv",
									simulations.filename().c_str(), 1);
	if (!picked) return;
	simulations.filename(picked);
	if (simulations.save() == 0)
		txt_simulations_filename->value(fl_filename_name(simulations.filename().c_str()));
}

// CALLED ONLY FROM THE COMP THREAD
static std::string str_szdb = "";
static void update_AWGN_rms(void *)
{
	inp_AWGN_rms->value(str_szdb.c_str());
}

static std::string str_simname = "";
static void update_simname(void *)
{
	txt_simulation->value(str_simname.c_str());
}

void process_modem_series(int run_type)
{
	int index = 0;
	std::string modem_name;

	batch_dir.assign(finp_output_wav_folder->value());

	if(batch_dir.empty())
		choose_batch_folder();

	if(batch_dir.empty()) return;

	fname_out_list.clear();
	fname_out_vals.clear();
	fname_out_sim_name.clear();

	for(index = progStatus.start_modem_index;
		index <= progStatus.end_modem_index; index++) {
		if(modem_name_by_index(modem_name, index)) {
			if(op_abort) return;
			if(fldigi_record_audio(modem_name))
				memset(&sim_vals, 0, sizeof(sim_vals));
				switch(run_type) {
					case BATCH_RUN:
						process_batch_items();
						break;

					case AWGN_RUN:
						process_AWGN_series();
						break;

					case PROCESS_RUN:
						process_run_simulation();
						break;
				}
		}

		if(op_abort) return;
		fldigi_play_audio();
	}
}

void process_AWGN_series()
{
	int upper = (int) cntr_High_dB->value();
	int lower = (int)  cntr_Low_dB->value();
	int step  = (int) cntr_Step_dB->value();
	char simname[100];
	char szdb[10];

	std::string basename;

	if(modem_series_flag) {
		basename = fname_in;
	} else if (btn_same_as_input_file->value()) {
		fname_in = input_filename;
		basename = fname_in;
	} else {
		string name = fl_filename_name(input_filename.c_str());
		basename = finp_output_wav_folder->value();
		basename.append(name);
	}

	if (basename.empty())
		return;

	size_t p = basename.find(".wav");
	if (p != string::npos) basename.erase(p);
	basename.append(".");

	Fl::awake(clear_AWGN);

	analyze_input();

	for (int db = lower; db <= upper; db += step) {
		snprintf(szdb, sizeof(szdb), "%d", db);

		str_szdb = szdb;
		Fl::awake(update_AWGN_rms);
		if (op_abort) break;

		if (szdb[0] == '-') szdb[0] = 'm';
		snprintf(simname, sizeof(simname), "AWGN_SN_%-s", szdb);

		str_simname = simname;
		Fl::awake(update_simname);

		fname_out.assign(basename);
		fname_out.append(simname).append(".wav");

		ofname = fname_out;
		Fl::awake(show_ofname);

		sim_vals.AWGN_on  = true;
		sim_vals.snrdb    = (float) db;

		if(modem_series_flag) {
			fname_out_list.push_back(fname_out);
		}

		generate_output(false);
	}

	Fl::awake(clear_main_dialog);
}

void AWGNseries_process()
{
	AWGN_process_dialog->hide();
	simulation = AWGN_SERIES;
}

void cancel_AWGNseries()
{
	AWGN_process_dialog->hide();
}

void AWGNseries_dialog()
{
	if (!AWGN_process_dialog) {
		AWGN_process_dialog = make_AWGNseries_dialog();
	}
	AWGN_process_dialog->show();
}

void cb_awgn_series_lower(void)
{
	progStatus.awgn_series_lower = cntr_Low_dB->value();
}

void cb_awgn_series_upper(void)
{
	progStatus.awgn_series_upper = cntr_High_dB->value();
}

void cb_awgn_series_step(void)
{
	progStatus.awgn_series_step = cntr_Step_dB->value();
}

void abort_simulation()
{
	op_abort = true;
}


void output_folder_select()
{
	string dir = fname_in;
	size_t p = dir.find(fl_filename_name(dir.c_str()));
	if (p != string::npos) dir.erase(p);

	char *szdir = fl_dir_chooser ( "Output wav file folder", dir.c_str(), 0 );

	if (szdir) {
		finp_output_wav_folder->value(szdir);
		finp_output_wav_folder->redraw();
	}
}

void close_output_dialog()
{
	select_output_dialog->hide();
}

void choose_batch_folder()
{
	if (!select_output_dialog) select_output_dialog = make_folder_dialog();
	string dir = fname_in;
	size_t p = dir.find(fl_filename_name(dir.c_str()));
	if (p != string::npos) dir.erase(p);
	batch_dir.assign(dir);
	finp_output_wav_folder->value(dir.c_str());
	finp_output_wav_folder->redraw();
	select_output_dialog->show();
}

// Modem Series
static inline void fldigi_offline_message(void)
{
	if(!fldigi_online) {
		fl_choice((const char *)"Fldigi doesn't appear to be running.",
				  (const char *) "Close", (const char *) 0, (const char *) 0);
	}
}

static inline void swap_selected_modems(void)
{
	int temp = progStatus.end_modem_index;
	progStatus.end_modem_index = progStatus.start_modem_index;
	progStatus.start_modem_index = temp;
	start_modem_choice->value(progStatus.start_modem_index);
	end_modem_choice->value(progStatus.end_modem_index);

	progStatus.start_modem_name.assign(start_modem_choice->text(progStatus.start_modem_index));
	progStatus.end_modem_name.assign(end_modem_choice->text(progStatus.end_modem_index));
}

void start_modem(void)
{
	progStatus.start_modem_index = start_modem_choice->value();

	if(progStatus.start_modem_index > progStatus.end_modem_index) {
		swap_selected_modems();
	} else {
		const Fl_Menu_Item * selected = start_modem_choice->mvalue();
		if(selected)
			progStatus.start_modem_name.assign(selected->label());
	}
}

void end_modem(void)
{
	progStatus.end_modem_index = end_modem_choice->value();

	if(progStatus.start_modem_index > progStatus.end_modem_index) {
		swap_selected_modems();
	} else {
		const Fl_Menu_Item * selected = end_modem_choice->mvalue();
		if(selected)
			progStatus.end_modem_name.assign(selected->label());
	}
}

void enable_ModemSeries(void)
{
	modem_series_tx_string.assign(limited_char_list, sizeof(limited_char_list));
	modem_series_flag = fldigi_online;

	if(!fldigi_online) {
		Output_Modem_Series_Status->value("Disabled");
		fldigi_offline_message();
	} else {
		Output_Modem_Series_Status->value("Enabled");
	}
}

void disable_ModemSeries(void)
{
	modem_series_flag = false;
	Output_Modem_Series_Status->value("Disabled");
}

void  close_ModemSeries(void)
{
	modem_series_dialog->hide();
}

void query_ModemSeries(void)
{
	populate_modem_list();
	fldigi_offline_message();
}

bool populate_modem_list(void)
{
	std::string modem_list;
	get_fldigi_modems(modem_list);

	modem_list_populated = false;

	if(modem_list.length()) {
		start_modem_choice->clear();
		start_modem_choice->add(modem_list.c_str());
		start_modem_choice->value(progStatus.start_modem_index);

		end_modem_choice->clear();
		end_modem_choice->add(modem_list.c_str());
		end_modem_choice->value(progStatus.end_modem_index);

		modem_list_populated = fldigi_online;
	}

	return modem_list_populated;
}

void ModemSeries_dialog(void)
{
	if(!modem_series_dialog) {
		modem_series_dialog = make_ModemSeries_dialog();
	}

	if(modem_series_dialog)
		modem_series_dialog->show();
}

void high_speed(void)
{
	progStatus.high_speed = ck_high_speed->value();
}

void XmlRpc_IP_dialog(void)
{
	if(!ip_address_dialog)
		ip_address_dialog = make_xmlrpc_ip_address();

	if(ip_address_dialog) {

		ip_address_dialog->show();
	}
}

void close_xmlrpc_ip_address_dialog(void)
{
	ip_address_dialog->hide();
}

void save_xmlrpc_ip_address_dialog(void)
{
	ip_address_dialog->hide();
}

void ip_address_text_field(void)
{
	progStatus.xmlrpc_ip_address.assign(inp_xmlrpc_ip_address->value());
}

void display_tx_duration_dialog(void)
{
	if(!tx_duration_dialog)
		tx_duration_dialog = make_tx_duration_dialog();

	if(tx_duration_dialog)
		tx_duration_dialog->show();
}

void close_tx_duration_dialog(void)
{
	if(tx_duration_dialog)
		tx_duration_dialog->hide();
}

void ip_port_no_text_field(void)
{
	progStatus.xmlrpc_ip_port_no = atoi(inp_xmlrpc_ip_port_no->value());
}

double extract_duration(std::string data, float &samplerate)
{
	const char *cPtr = data.c_str();
	int count = data.length();
	int cc = 0;
	float duration = 0.0;
	long no_of_samples = 0;
	float sr = 0.0;

	if(!count) return 0.0;

	cc = sscanf(cPtr, "%ld : %f : %f", &no_of_samples, &sr, &duration);

	samplerate = sr;
	return duration;
}

int process_received_text(std::string filename, std::string &reply, _vals &_sim_vals, std::string &sim_name)
{
	std::string lower_case;
	std::string save_data_fn;
	save_data_fn.assign(batch_dir).append(current_modem).append(".txt");
	std::string ret_file_name;
	std::string ret_data;
	std::string file_marker  = "FILE: ";
	std::string start_marker = "[START]";
	std::string end_marker   = "[END]";

	FILE *fd = (FILE *)0;
	char * write_mode = (char *) "";
	int count = 0;
	int copy_found = 0;
	static char seperator[] = "------------------------------------------------------------------------";
	int max_line_width = 0;
	int max_line_index = 0;
	int no_of_matches  = 0;
	double quality_rx   = 0.0;

#ifdef WIN32
	write_mode = (char *) "at";
#else
	write_mode = (char *) "a";
#endif

	lower_case.assign(seperator);
	max_line_width = lower_case.size();

	if(max_line_width > 79)
		max_line_width = 79;

	if(save_data_fn.empty()) return 0;

	fd = fopen(save_data_fn.c_str(), write_mode);

	fprintf(fd, "\n%s\n", seperator);

	size_t pos1, pos2;

	pos1 = reply.find(file_marker);
	pos2 = reply.find(start_marker);

	ret_file_name.clear();
	if(pos1 < pos2) {
		for(size_t j = pos1; j < pos2; j++)
			ret_file_name += reply[j];
	}

	pos1 = reply.find(start_marker);
	pos2 = reply.find(end_marker) + end_marker.size();

	ret_data.clear();
	if(pos1 < pos2) {
		for(size_t j = pos1; j < pos2; j++)
			ret_data += reply[j];
	}

	// Convert to lower case.
	count = ret_data.size();
	lower_case.clear();

	for(int i = 0; i < count; i++)
		lower_case += tolower(ret_data[i]);

	no_of_matches = 0;
	pos1 = 0;
	do {
		pos1 = lower_case.find(limited_char_list, pos1);
		if(pos1 != std::string::npos) {
			no_of_matches++;
			pos1 += (sizeof(limited_char_list) - 1);
		} else {
			break;
		}
	} while(1);

	fprintf(fd, "%s\n", ret_file_name.c_str());

	if(no_of_matches) {
		fprintf(fd, "***********************\n");
		fprintf(fd, "* VALID DATA RECEIVED *\n");
		fprintf(fd, "***********************\n\n");
		copy_found = 1;
	}

	fprintf(fd, "        TX STRING: %s\n", limited_char_list);
	fprintf(fd, "         TX COUNT: %d\n", char_tx_repeat_count + 1);
	fprintf(fd, "         RX COUNT: %d\n", no_of_matches);

	quality_rx = (double) no_of_matches;
	quality_rx /= ((double) char_tx_repeat_count + 1.0);
	quality_rx *= 100.0;

	fprintf(fd, "  RX WORD QUALITY: %1.2f%%\n\n", quality_rx);

	if(!sim_name.empty()) {
		fprintf(fd, "       SIMULATION: %s\n", sim_name.c_str());
	}

	if(_sim_vals.p0.active) {
		fprintf(fd, "           PATH 0: Spread %1.2f Offset %1.2f\n",
			_sim_vals.p0.spread, _sim_vals.p0.offset);
	}

	if(_sim_vals.p1.active) {
		fprintf(fd, "           PATH 1: Spread %1.2f Offset %1.2f Delay %1.2f \n",
			_sim_vals.d.delay1, _sim_vals.p1.spread, _sim_vals.p1.offset);
	}

	if(_sim_vals.p2.active) {
		fprintf(fd, "           PATH 2: Spread %1.2f Offset %1.2f Delay %1.2f \n",
				_sim_vals.p2.spread, _sim_vals.p2.offset, _sim_vals.d.delay2);
	}

	if(_sim_vals.AWGN_on) {
		fprintf(fd, "         AWGN SNR: %1.2f dB\n", _sim_vals.snrdb);
	}

	fprintf(fd, "\nModem Sample Rate: %1.2f\n", modem_sample_rate);
	fprintf(fd, " File Sample Rate: %1.2f\n", file_sample_rate);

	max_line_index = 0;
	count = ret_data.size();
	lower_case.clear();

	fprintf(fd, "\nRECEIVED DATA:\n\n");

	// Filter and format data
	for(int i = 0; i < count; i++) {
		if(ret_data[i] < ' ' || ret_data[i] > '~') continue;
		lower_case += ret_data[i];
		max_line_index++;
		if(max_line_index >= max_line_width) {
			fprintf(fd, "%s\n", lower_case.c_str());
			max_line_index = 0;
			lower_case.clear();
		}
	}

	if(lower_case.size())
		fprintf(fd, "%s\n", lower_case.c_str());

	fclose(fd);

	return no_of_matches;
}

long wait_ms = 0;

bool toggle_modem(std::string modem)
{
	std::string reply;
	std::string swap_modem;
	long ms_delay = 250;
	int tries = 0;
	int total_delay = 4000;
	int try_count = total_delay / ms_delay;
	int try_half_count = try_count >> 1;

	if(current_modem == "CW")
		swap_modem.assign("RTTY");
	else
		swap_modem.assign("CW");

	tries = try_count;
	send_new_modem(swap_modem);
	while(tries > 0) {
		if(tries < try_half_count) {
			send_new_modem(swap_modem);
			try_half_count >>= 1;
		}
		get_fldigi_modem(reply);
		if(reply == swap_modem) break;
		tries--;
		MilliSleep(ms_delay);
	}

	try_half_count = try_count >> 1;
	tries = try_count;
	send_new_modem(modem);
	while(tries > 0) {
		if(tries < try_half_count) {
			send_new_modem(swap_modem);
			try_half_count >>= 1;
		}
		get_fldigi_modem(reply);
		if(reply == modem) break;
		tries--;
		MilliSleep(ms_delay);
	}

	return true;
}

struct special_case_modems {
	char *modem;
	float data_scale;
};

#define MT63_TIME 2.50
#define THOR_TIME 2.50

struct special_case_modems spc_modems[] =  {
	{ (char *) "MFSK128L",   4.00 },
	{ (char *) "MFSK64L",    2.75 },
	{ (char *) "MT63-500L",  MT63_TIME },
	{ (char *) "MT63-500S",  MT63_TIME },
	{ (char *) "MT63-1KL",   MT63_TIME },
	{ (char *) "MT63-1KS",   MT63_TIME },
	{ (char *) "MT63-2KL",   MT63_TIME },
	{ (char *) "MT63-2KS",   MT63_TIME },
	{ (char *) "8PSK125F",   2.50 },
	{ (char *) "8PSK125FL",  2.50 },
	{ (char *) "THOR4",      THOR_TIME },
	{ (char *) "THOR5",      THOR_TIME },
	{ (char *) "THOR8",      THOR_TIME },
	{ (char *) "THOR11",     THOR_TIME },
	{ (char *) "THOR16",     THOR_TIME },
	{ (char *) "THOR25x4",   THOR_TIME },
	{ (char *) "THOR50x1",   THOR_TIME },
	{ (char *) "THOR50x2",   THOR_TIME },
	{ (char *) "THOR100",    THOR_TIME }
};

// Reading bit streams with 100% errors may contain more characters
// then what was sent. Increase read amount to compensate. This has
// the additional benefit when having to deal with modems with high
// latency. A table of special cases provided for the modems
// with very high latency if the base read count is not enough.
// Read errors either in front or behind valid data are highly
// dependent on the quality of the signal and the modem latency.
float modem_data_scale(std::string modem)
{
	int modem_list_count = sizeof(spc_modems) / sizeof(struct special_case_modems);
	float data_scale = 1.8;

	for(int i = 0; i < modem_list_count; i++) {
		if(modem == spc_modems[i].modem) {
			data_scale = spc_modems[i].data_scale;
		}
	}

	return data_scale;
}

void start_busy_timer(std::string msg)
{
	int count = 0;
	static char _buf[128];

	count = sizeof(_buf);
	memset(_buf, 0, count);
	if(count > msg.size())
		count = msg.size();
	memcpy(_buf, msg.c_str(), count);
	Fl::awake(start_blink_progress, (void *) _buf);
}

void busy_timer_label(std::string msg)
{
	int count = 0;
	static char _buf[128];

	count = sizeof(_buf);
	memset(_buf, 0, count);
	if(count > msg.size())
		count = msg.size();
	memcpy(_buf, msg.c_str(), count);
	Fl::awake(update_progress_label, (void *) _buf);
}

void stop_busy_timer(void)
{
	Fl::awake(stop_blink_progress);
}

bool fldigi_record_audio(std::string modem)
{
	std::string reply;
	std::string data;
	std::string msg;
	float scale = 0.0;
	// float remainder = 0;
	std::string filename_path;
	float duration = (float) progStatus.tx_duration;
	float data_duration = 0;
	float dummy = 0;
	if(modem.empty()) return false;

	current_modem.assign(modem);

	toggle_modem(modem);

	fname_out_list.clear();
	filename_path.assign(batch_dir).append(modem).append(SOUND_FILE_EXT);
	fname_in.assign(modem).append(SOUND_FILE_EXT);

	start_busy_timer(fname_in);

	data.assign(limited_char_list);
	get_tx_timing(data, reply);

	if(!reply.empty()) {
		duration = extract_duration(reply, modem_sample_rate);
	}

	data.assign(limited_char_list).append(limited_char_list);
	get_tx_timing(data, reply);

	if(!reply.empty()) {
		data_duration = extract_duration(reply, dummy);
	}

	data_duration -= duration; // Remove modem over head time.

	char_tx_repeat_count = 0;

	if(data_duration < ((float) progStatus.tx_duration)) {
		char_tx_repeat_count = scale = ((float) progStatus.tx_duration) / data_duration;
		scale -= char_tx_repeat_count;
		if(scale > 0.0)
			char_tx_repeat_count++;
	}


	if(char_tx_repeat_count < 0)
		char_tx_repeat_count = 0;

	data.assign(limited_char_list);
	for(int i = 0; i < char_tx_repeat_count; i++) {
		data.append(limited_char_list);
	}

	// Some modem have a long decode process before valid characters
	// are returned. Adjust read length as required.

	int _read_count = modem_data_scale(modem) * data.size();
	set_read_count(_read_count);

	get_tx_timing(data, reply);

	if(!reply.empty()) {
		duration = extract_duration(reply, dummy);
	}

	data.append("^r");             // Add terminate TX character sequence.

	wait_ms = (long) (duration * 1000.0);

	send_audio_record_file(filename_path);

	// Send TX data for transmitting.
	send_report(data);

	MilliSleep(250);

	// Transmit
	send_tx();

	MilliSleep(wait_ms);

	while(reply != "RX") {
		get_trx_state(reply);
		MilliSleep(250);
		if(op_abort) return false;
	};

	send_audio_record_stop();
	stop_busy_timer();

	MilliSleep(250);

	return true;
}

bool fldigi_play_audio(void)
{
	std::string reply;
	std::string data;
	std::string filename_path;
	std::string swap_modem;
	std::string msg = "";
	std::string sim_name = "";
	int  file_count     = fname_out_list.size();
	int  sim_val_count  = fname_out_vals.size();
	int  sim_name_count = fname_out_sim_name.size();

	_vals processed_sim_vals;
	// long file_delay = 1000;
	long xmlrpc_delay = 1500;
	
	do {
		get_trx_state(reply);
		MilliSleep(xmlrpc_delay);
		if(op_abort) return false;
	} while(reply != "RX");
	
	if(file_count) {
		start_busy_timer(msg);
	}
	
	// Instruct FLDIGI linsim data should be recorded.
	set_record_data(true);
	
	for(int i = 0; i < file_count; i++) {
		
		busy_timer_label(fname_out_list[i]);
		
		filename_path.assign(batch_dir).append(fname_out_list[i]);

		if(i < sim_val_count)
			processed_sim_vals = fname_out_vals[i];
		else
			memset(&processed_sim_vals, 0, sizeof(processed_sim_vals));

		if(i < sim_name_count)
			sim_name.assign(fname_out_sim_name[i]);
		else
			sim_name.clear();

		if(!filename_path.empty()) {
			
			// Flush modem bit stream before reading data.
			toggle_modem(current_modem);
			
			send_audio_play_file(filename_path);
			
			if(progStatus.high_speed) {
				send_modem_highspeed(true);
				MilliSleep(wait_ms >> 3);
			} else {
				MilliSleep(wait_ms);
			}
			
			do {
				get_linsim_status(reply);
				MilliSleep(xmlrpc_delay);
				if(reply.empty()) return false;
				if(op_abort) return false;
			} while(reply == "Busy");
		}
		
		get_linsim_rx_data(reply);
		process_received_text(filename_path, reply, processed_sim_vals, sim_name);
	}
	
	// Instruct FLDIGI linsim data should not be recorded.
	set_record_data(false);
	
	fname_out_list.clear();
	Fl::awake(stop_blink_progress);
	
	return true;
}

inline bool modem_name_by_index(std::string &modem, int index)
{
	const char * modem_name = start_modem_choice->text(index);
	
	modem.assign(modem_name);
	
	if(modem.empty())
		return false;
	
	return true;
}


char szAbout[200];
void about()
{
	snprintf (szAbout, sizeof(szAbout),"\
			  %s\n\n\
			  HF path simulation program\n\
			  Author: W1HKJ\n\n\
			  Based on Moe Wheatley's PathSim, @ AE4JY\n\n\
			  Report problems to %s", PACKAGE_STRING, PACKAGE_BUGREPORT);
	fl_message("%s", szAbout);
}

#include "support/guide.cxx"

void guideURL()
{
	string deffname = HomeDir;
	deffname.append("linsim_guide.html");
	ofstream f(deffname.c_str());
	if (!f)
		return;
	f << szGuide;
	f.close();
#ifndef __WOE32__
	visit_URL((void *)deffname.insert(0, "file://").c_str());
#else
	visit_URL((void *)deffname.c_str());
#endif
}

