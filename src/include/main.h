//======================================================================
//  linsim; main.h
//    Author: Dave Freese, W1HKJ <w1hkj@w1hkj.com>
//
//  based on the program RTTY Compare by
//    Alex Shovkoplyas, VE3NEA ve3nea@dxatlas.com
//  Original written in Pascal and distributed only for Windows OS
//
//  This program is distributed under the GPL license
//======================================================================

#include <string>

#include "linsim_ui.h"
#include "sim_vals.h"
#include "sim.h"
#include "csv.h"

extern string HomeDir;

extern csvdb simulations;
extern SIM sim_test;
extern _vals sim_vals;
extern std::string fname_in;
extern std::string fname_out;

extern void clean_exit();
extern int parse_args(int argc, char **argv, int& idx);

extern void select_simulation();
extern void add_simulation();
extern void run_simulation();

extern void tbl_simulations_selected();
extern void cancel_selection();
extern void delete_selection(int);
extern void update_selection(int);
extern void select_entry(int);

extern void load_simulation_set();
extern void save_simulation_set();
extern void save_simulation_set_as();
extern void populate_batch_selector();
extern void cancel_batch_process();
extern void batch_process_items();
extern void open_batch_process_dialog();
extern void clear_all_simulation();
extern void select_all_simulations();
