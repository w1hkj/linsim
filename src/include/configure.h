// ----------------------------------------------------------------------------
// Copyright (C) 2014-2016
//              David Freese, W1HKJ
// Copyright (C) 2016
//              Robert Stiles, KK5VD
//
// This file is part of linSIM
//
// fldigi is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// fldigi is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#ifndef _status_H
#define _status_H

#include <string>

#include "main.h"

struct status {
	std::string xmlrpc_ip_address;
	int xmlrpc_ip_port_no;
	int start_modem_index;
	int end_modem_index;
	std::string start_modem_name;
	std::string end_modem_name;
	int high_speed;

	int   path0_active;
	float path0_spread;
	float path0_offset;

	int   path1_active;
	float path1_delay;
	float path1_spread;
	float path1_offset;

	int   path2_active;
	float path2_delay;
	float path2_spread;
	float path2_offset;

	int   awgn_active;
	float awgn_snr;

	int awgn_series_upper;
	int awgn_series_lower;
	int awgn_series_step;

	std::string batch_directory;
	std::string models;
	std::string simulations;

	int sample_rate_menu_index;

	int tx_duration;

	void initLastState(void);
	void saveLastState(void);
	void loadLastState(void);
};

extern status progStatus;

#endif
