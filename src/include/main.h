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
extern std::string fname_out;
extern std::string input_filename;
extern std::string output_filename;

extern csvdb simulations;
extern SIM sim_test;
extern _vals sim_vals;
extern float file_sample_rate;

extern void clean_exit(void);
extern int parse_args(int argc, char **argv, int& idx);

extern void select_simulation(void);
extern void add_simulation(void);
extern void run_simulation(void);

extern void tbl_simulations_selected(void);
extern void cancel_selection(void);
extern void delete_selection(int);
extern void update_selection(int);
extern void select_entry(int);

extern void load_simulation_set(void);
extern void save_simulation_set(void);
extern void save_simulation_set_as(void);
extern void populate_batch_selector(void);
extern void cancel_batch_process(void);
extern void batch_process_items(void);
extern void open_batch_process_dialog(void);
extern void clear_all_simulation(void);
extern void select_all_simulations(void);
extern void abort_simulation(void);
extern void clear_main_dialog(void *);
extern void display_tx_duration_dialog(void);
extern void close_tx_duration_dialog(void);
extern void AWGNseries_dialog(void);
extern void AWGNseries_process(void);
extern void cancel_AWGNseries(void);

extern void choose_batch_folder(void);
extern void output_folder_select(void);
extern void close_output_dialog(void);

extern void about(void);
extern void guideURL(void);

// Modem Series
extern void start_modem(void);
extern void end_modem(void);
extern void enable_ModemSeries(void);
extern void disable_ModemSeries(void);
extern void close_ModemSeries(void);
extern void query_ModemSeries();
extern bool populate_modem_list(void);
extern bool modem_list_populated;
extern void ModemSeries_dialog();
extern void process_AWGN_series_modem_series(void);
extern void process_run_simulation_modem_series(void);
extern void process_batch_items_modem_series(void);
extern void high_speed(void);
// End Modem Series

void ip_address_text_field(void);
void ip_port_no_text_field(void);
void save_xmlrpc_ip_address_dialog(void);
void close_xmlrpc_ip_address_dialog(void);
void XmlRpc_IP_dialog(void);
void select_models_dialog(void);
void cb_select_models(void);
void select_model_list(void);
void close_models_dialog(void);
void cb_awgn_series_lower(void);
void cb_awgn_series_upper(void);
void cb_awgn_series_step(void);