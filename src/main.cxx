//======================================================================
//  linsim; main
//    Author: Dave Freese, W1HKJ <w1hkj@w1hkj.com>
//
//  based on the program RTTY PathSim by Moe Wheatley AE4JY
//
//  This program is distributed under the GPL license
//======================================================================

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

#include "config.h"
#include "sound.h"
#include "util.h"

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
void process_run_simulation();
void process_batch_items();
void process_AWGN_series();

string BaseDir = "";
string HomeDir = "";

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
string fname_in;
string fname_out;
string fname_temp;

Fl_Double_Window *simulator_selector = (Fl_Double_Window *)0;
Fl_Double_Window *batch_process_selector = (Fl_Double_Window *)0;
Fl_Double_Window *AWGN_process_dialog = (Fl_Double_Window *)0;
Fl_Double_Window *select_output_dialog = (Fl_Double_Window *)0;

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
					process_run_simulation();
					simulation = IDLE;
					Fl::awake(reset_buttons);
					Fl::awake(clear_main_dialog);
					op_abort = false;
					break;
				case BATCH :
					process_batch_items();
					simulation = IDLE;
					Fl::awake(reset_buttons);
					Fl::awake(clear_main_dialog);
					op_abort = false;
					break;
				case AWGN_SERIES :
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
	if (!fname_temp.empty()) remove(fname_temp.c_str());

	if (simulator_selector) simulator_selector->hide();
	if (batch_process_selector)  batch_process_selector->hide();
	if (AWGN_process_dialog) AWGN_process_dialog->hide();
	if (select_output_dialog) select_output_dialog->hide();

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

	linsim_window = make_linsim_window();
	simulator_selector = make_selector_dialog();
	batch_process_selector = make_batch_selector_dialog();
	AWGN_process_dialog = make_AWGNseries_dialog();
	select_output_dialog = make_folder_dialog();

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

	start_computation_thread();

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

size_t buffs_read;

// CALLED ONLY FROM THE COMP THREAD
void analyze_input()
{
	if (fname_in.empty()) return;
	string dir = fname_in;
	size_t p = dir.find(fl_filename_name(dir.c_str()));
	if (p != string::npos) dir.erase(p);
	fname_temp = dir;
	fname_temp.append("linsim.tmp");

	double buffer[MAX_BUF_SIZE];

	sim_test.sound_in.open(fname_in, SoundFile::READ);

	size_t r = 0;
	sim_test.signal_rms = 0.0;

	str_progress = "Analyzing input";
	Fl::awake(set_progress);
	if (op_abort) return;

	sim_test.signal_rms = 0.0;
	sim_test.num_buffs = 0;

	buffs_read = 0;
	sim_test.signal_peak = 0;

	size_t num_buffs = sim_test.sound_in.size() / MAX_BUF_SIZE;

	ofstream tmp(fname_temp.c_str());
	if (!tmp) {
		throw "Cannot open temp file";
	}

	memset(buffer, 0, MAX_BUF_SIZE * sizeof(double));
	while ((r = sim_test.sound_in.read(buffer, MAX_BUF_SIZE)) > 0) {
		sim_test.measure_rms(buffer, MAX_BUF_SIZE);
		buffs_read++;
		memset(buffer, 0, MAX_BUF_SIZE * sizeof(double));
		f_progress = 1.0 * buffs_read / num_buffs;
		Fl::awake(update_progress);
		if (op_abort) break;
	}
	tmp.close();

	if (sim_test.signal_rms < 1e-6) sim_test.signal_rms = 1e-6;

	sim_test.sound_in.close();
}

extern int linsim_samplerate;

// CALLED ONLY FROM THE COMP THREAD
void generate_output()
{
	double buffer[MAX_BUF_SIZE];

	if (fname_out.empty()) return;

	update_sim_vals();

	sim_test.AWGN(sim_vals.AWGN_on);
	sim_test.SetsnrValue(sim_vals.snrdb);

	init_BPFir(linsim_samplerate);
	sim_test.init(linsim_samplerate, sim_vals.p0, sim_vals.p1, sim_vals.p2, sim_vals.d);

	set_output_sr();

	sim_test.noise_rms = sim_test.signal_rms / sim_test.snr;

	ifstream tmp(fname_temp.c_str());
	if (!tmp) {
		throw "Cannot open temp file";
	}

	sim_test.sound_in.open(fname_in, SoundFile::READ);

	memset(buffer, 0, MAX_BUF_SIZE * sizeof(double));
	size_t pq = 0;
	size_t r = 0;
	double maxsig = 1e-6;

	f_progress = 0.0;
	Fl::awake(update_progress);
	str_progress = "Running Simulation";
	Fl::awake(set_progress);

	while ((r = sim_test.sound_in.read(buffer, MAX_BUF_SIZE)) > 0) {
		f_progress = 1.0 * (++pq) / buffs_read;
		Fl::awake(update_progress);
		if (op_abort) break;
		sim_test.Process(buffer, MAX_BUF_SIZE);
		for (int i = 0; i < MAX_BUF_SIZE; i++)
			if (fabs(buffer[i]) > maxsig) maxsig = fabs(buffer[i]);
		memset(buffer, 0, MAX_BUF_SIZE * sizeof(double));
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
	memset(buffer, 0, MAX_BUF_SIZE * sizeof(double));
	while ((r = sim_test.sound_in.read(buffer, MAX_BUF_SIZE)) > 0) {
		f_progress = 1.0 * (++pq) / buffs_read;
		Fl::awake(update_progress);
		if (op_abort) break;
		sim_test.Process(buffer, MAX_BUF_SIZE);
		for (int i = 0; i < MAX_BUF_SIZE; i++)
			buffer[i] *= (0.9 / maxsig);
		sim_test.sound_out.write(buffer, MAX_BUF_SIZE);
		memset(buffer, 0, MAX_BUF_SIZE * sizeof(double));
	}

	sim_test.sound_out.close();
	sim_test.sound_in.close();

	str_progress = "";
	Fl::awake(set_progress);
	f_progress = 0.0;
	Fl::awake(update_progress);
}

// CALLED ONLY FROM THE COMP THREAD
void process_run_simulation()
{
	btn_test->hide();
	btn_abort->show();
	analyze_input();
	generate_output();
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
	fname_in = fname_out = fname_temp = "";

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
void process_batch_items()
{
	printf("run batch\n");

	string basename;
	string simname;

	if (btn_same_as_input_file->value())
		basename = fname_in;
	else {
		string name = fl_filename_name(fname_in.c_str());
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

			generate_output();
		}
	}
	Fl::awake(clear_main_dialog);
}

void batch_process_items()
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

void process_AWGN_series()
{
	printf("run AWGN series\n");
	int upper = (int)cntr_High_dB->value();
	int lower = (int)cntr_Low_dB->value();
	int step  = (int)cntr_Step_dB->value();
	char simname[100];
	char szdb[10];

	string basename;
	if (btn_same_as_input_file->value())
		basename = fname_in;
	else {
		string name = fl_filename_name(fname_in.c_str());
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

		generate_output();
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
	if (!AWGN_process_dialog) AWGN_process_dialog = make_AWGNseries_dialog();
	AWGN_process_dialog->show();
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
	finp_output_wav_folder->value(dir.c_str());
	finp_output_wav_folder->redraw();
	select_output_dialog->show();
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

