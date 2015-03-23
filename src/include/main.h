//======================================================================
//  linsim; main.h
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
//======================================================================

#include <string>

#include "linsim_ui.h"
#include "sim_vals.h"
#include "sim.h"
#include "csv.h"

extern std::string BaseDir;
extern std::string HomeDir;
extern std::string fname_in;
extern std::string fname_out;

extern csvdb simulations;
extern SIM sim_test;
extern _vals sim_vals;

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
extern void abort_simulation();
extern void clear_main_dialog(void *d = 0);

extern void AWGNseries_dialog();
extern void AWGNseries_process();
extern void cancel_AWGNseries();

extern void choose_batch_folder();
extern void output_folder_select();
extern void close_output_dialog();

extern void about();
extern void guideURL();



