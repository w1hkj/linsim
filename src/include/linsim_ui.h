// generated by Fast Light User Interface Designer (fluid) version 1.0302

#ifndef linsim_ui_h
#define linsim_ui_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *linsim_window;
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
extern Fl_Button *btn_select_input;
#include <FL/Fl_Output.H>
extern Fl_Output *txt_input_file;
extern Fl_Button *btn_select_output;
extern Fl_Output *txt_output_file;
extern Fl_Button *btn_select_simulation;
#include <FL/Fl_Input.H>
extern Fl_Input *txt_simulation;
#include <FL/Fl_Group.H>
#include <FL/Fl_Check_Button.H>
extern Fl_Check_Button *p0_on;
#include <FL/Fl_Float_Input.H>
extern Fl_Float_Input *inp_spread0;
extern Fl_Float_Input *inp_offset0;
extern Fl_Check_Button *p1_on;
extern Fl_Float_Input *inp_delay1;
extern Fl_Float_Input *inp_spread1;
extern Fl_Float_Input *inp_offset1;
extern Fl_Check_Button *p2_on;
extern Fl_Float_Input *inp_delay2;
extern Fl_Float_Input *inp_spread2;
extern Fl_Float_Input *inp_offset2;
extern Fl_Check_Button *inp_AWGN_on;
extern Fl_Float_Input *inp_AWGN_rms;
#include <FL/Fl_Progress.H>
extern Fl_Progress *progress;
extern Fl_Button *btn_test;
Fl_Double_Window* make_linsim_window();
extern Fl_Menu_Item menu_[];
#define mnuAddSimulation (menu_+4)
#define mnuSelectSimulation (menu_+5)
#include <FL/Fl_Return_Button.H>
extern Fl_Return_Button *btn_select;
#include <FL/Fl_Browser.H>
extern Fl_Browser *tbl_simulations;
extern Fl_Button *btn_delete_selection;
extern Fl_Button *btn_update_selection;
Fl_Double_Window* make_selector_dialog();
#endif
