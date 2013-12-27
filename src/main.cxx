//======================================================================
//  linsim; main
//    Author: Dave Freese, W1HKJ <w1hkj@w1hkj.com>
//
//  based on the program RTTY Compare by
//    Alex Shovkoplyas, VE3NEA ve3nea@dxatlas.com
//  Original written in Pascal and distributed only for Windows OS
//
//  This program is distributed under the GPL license
//======================================================================

#include "main.h"
#include <string>
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

#include "config.h"

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

#include <FL/x.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_File_Chooser.H>

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
std::string fname_in;
std::string fname_out;

Fl_Double_Window *simulator_selector = (Fl_Double_Window *)0;

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

void clean_exit()
{
	simulations.save();
	if (simulator_selector) simulator_selector->hide();
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
static void fatal_error(string sz_error)
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

	Fl::visual (FL_DOUBLE|FL_INDEX);

	sprintf (title, "linsim %s", linsim_VERSION);
	linsim_window->label(title);
	linsim_window->callback(exit_main);

#ifdef __WOE32__
	char dirbuf[FL_PATH_MAX + 1];
	fl_filename_expand(dirbuf, sizeof(dirbuf) -1, "$USERPROFILE/");
	HomeDir.assign(dirbuf).append("linsim.files/");
#else
	char dirbuf[FL_PATH_MAX + 1];
	fl_filename_expand(dirbuf, sizeof(dirbuf) -1, "$HOME/");
	HomeDir.assign(dirbuf).append(".linsim/");
#endif

	if (mkdir(HomeDir.c_str(), 0777) == -1 && errno != EEXIST) {
		string s = "Could not create linsim directory ";
		s.append(HomeDir);
		fatal_error(s);
	}

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

	simulations.load();

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

void run_simulation()
{
	double buffer[MAX_BUF_SIZE];

	sim_test.AWGN(sim_vals.AWGN_on);
	sim_test.SetsnrValue(sim_vals.snrdb);
	sim_test.init(8000.0, sim_vals.p0, sim_vals.p0, sim_vals.p1, sim_vals.d);

	if (fname_in.empty() || fname_out.empty()) return;
		sim_test.sound_in.open(fname_in, SoundFile::READ);
		size_t fsize = 0, r = 0, partial = 0;
		sim_test.signal_rms = 0.0;
		while ((r = sim_test.sound_in.read(buffer, MAX_BUF_SIZE)) > 0) {
			sim_test.measure_rms(buffer, MAX_BUF_SIZE);
			fsize += r;
		}
		sim_test.sound_in.rewind();
		sim_test.sound_out.open(fname_out, SoundFile::WRITE);
		progress->value(0);
		progress->minimum(0);
		progress->maximum(fsize * 1.0);
		while ((r = sim_test.sound_in.read(buffer, MAX_BUF_SIZE)) > 0) {
			partial += r;
			if ((partial / r) % 4 == 0) {
				progress->value(partial*1.0);
				progress->redraw();
				Fl::flush();
		}
		sim_test.Process(buffer, r);
		sim_test.sound_out.write(buffer, r);
	}
	progress->value(fsize * 1.0);
	sim_test.sound_in.close();
	sim_test.sound_out.close();
	progress->value(0);
	progress->redraw();
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

