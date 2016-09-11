// =====================================================================
//
// xmlrpc_client.h
//
// Author: Dave Freese, W1HKJ
// Copyright: 2010 to 2014
//
// This file is part of FLAMP.
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef XML_IO_H
#define XML_IO_H

#include <string>

void start_xmlrpc(void);
void terminate_xmlrpc(void);

extern void get_char_rates(std::string &reply);
extern void get_char_timing(int character, std::string &reply);
extern void get_rx_data(std::string &reply);
extern void get_trx_state(std::string &reply);
extern void get_tx_char_n_timing(int character, int count, std::string &reply);
extern void get_tx_duration(std::string &reply);
extern void get_tx_timing(std::string &data, std::string &reply);
extern void get_fldigi_modem(std::string &reply);
extern void get_fldigi_modems(std::string &reply);
extern void get_audio_play_rec_status(std::string &reply);

extern void get_linsim_status(std::string &status);
extern void get_linsim_rx_data(std::string &data);

extern bool set_record_data(bool value);
extern bool set_read_count(int count);

extern bool send_modem_highspeed(bool state);
extern bool send_add_tx_queue(std::string &data);
extern bool send_audio_play_file(std::string &filename_path);
extern bool send_audio_record_file(std::string &filename_path);
extern bool send_audio_record_stop(void);

extern void * xmlrpc_loop(void *d);
extern void close_xmlrpc();
extern void open_xmlrpc();
extern bool send_abort(void);
extern bool send_clear_rx(void);
extern bool send_clear_tx(void);
extern bool send_new_modem(std::string &modem);
extern bool send_report(std::string &report);
extern bool send_rx(void);
extern bool send_tx(void);
extern void set_xmlrpc_timeout_default(void);
extern void set_xmlrpc_timeout(double value);


#endif
