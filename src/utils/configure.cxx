// ----------------------------------------------------------------------------
// status.cxx
//
// Copyright (C) 2007-2016
//		Dave Freese, W1HKJ
// Copyright (C) 2008-2010
//		Stelios Bounanos, M0GLD
// Copyright (C) 2016
//		Robert Stiles, KK5VD
//
// This file is part of linsim.
//
// Fldigi is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Fldigi is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fldigi.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#include <config.h>

#include <iostream>
#include <fstream>
#include <string>

#include <FL/Fl_Preferences.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Choice.H>

#include "configure.h"
#include "main.h"
#include "linsim_ui.h"

#define STATUS_FILENAME "status"
#define DOMAIN_NAME     "w1hkj.com"
#define PACKAGE_NAME    "linsim"

status progStatus = {
	"127.0.0.1", // std::string xmlrpc_ip_address;
	7362,        // int xmlrpc_ip_port_n;
	0,           // int start_modem_index;
	0,           // int end_modem_index
	"CW",        // std::string start_modem_name;
	"CW",        // std::string end_modem_name;
	0,           // int high_speed;
	0,           // int path0_active;
	1.0,         // float path0_spread;
	0.0,         // float path0_offset;
	0,           // int path1_active;
	0.0,         // float path1_delay;
	1.0,         // float path1_spread;
	0.0,         // float path1_offset;
	0,           // int path2_active;
	0.0,         // float path2_delay;
	1.0,         // float path2_spread;
	0.0,         // float path2_offset;
	0,           // int awgn_active;
	0.0,         // float awgn_snr;
	20,          // int awgn_series_upper;
	0,           // int awgn_series_lower;
	1,           // int awgn_series_step;
	"",          // std::string batch_directory;
	"linsim.simulations.csv", // std::string models;
	"",          // std::string simulations;
	0,           // int sample_rate_menu_index;
	4            // int tx_duration;
};

void status::saveLastState(void)
{
	Fl_Preferences spref(HomeDir.c_str(), DOMAIN_NAME, PACKAGE_NAME);

	spref.set("xmlrpc_ip_address", xmlrpc_ip_address.c_str());
	spref.set("xmlrpc_ip_port_no", xmlrpc_ip_port_no);
	spref.set("start_modem_index", start_modem_index);
	spref.set("end_modem_index",   end_modem_index);
	spref.set("start_modem_name",  start_modem_name.c_str());
	spref.set("end_modem_name",    end_modem_name.c_str());
	spref.set("high_speed",        high_speed);

	spref.set("path0_active",      path0_active);
	spref.set("path0_spread",      path0_spread);
	spref.set("path0_offset",      path0_offset);

	spref.set("path1_active",      path1_active);
	spref.set("path1_delay",       path1_delay);
	spref.set("path1_spread",      path1_spread);
	spref.set("path1_offset",      path1_offset);

	spref.set("path2_active",      path1_active);
	spref.set("path2_delay",       path1_delay);
	spref.set("path2_spread",      path1_spread);
	spref.set("path2_offset",      path1_offset);

	spref.set("awgn_active",       awgn_active);
	spref.set("awgn_snr",          awgn_snr);

	spref.set("awgn_series_upper", awgn_series_upper);
	spref.set("awgn_series_lower", awgn_series_lower);
	spref.set("awgn_series_step",  awgn_series_step);

	spref.set("batch_directory",   batch_directory.c_str());
	spref.set("models",            models.c_str());
	spref.set("simulations",       simulations.c_str());

	spref.set("sample_rate_menu_index", sample_rate_menu_index);

	spref.set("tx_duration", tx_duration);
}

void status::loadLastState(void)
{
	Fl_Preferences spref(HomeDir.c_str(), DOMAIN_NAME, PACKAGE_NAME);

	int i;
	float f;
	char strbuff[1000];

	memset(strbuff, 0, sizeof(strbuff));
	spref.get("xmlrpc_ip_address", strbuff, xmlrpc_ip_address.c_str(), sizeof(strbuff) - 1);
	xmlrpc_ip_address = strbuff;

	spref.get("xmlrpc_ip_port_no", i, xmlrpc_ip_port_no); xmlrpc_ip_port_no = i;
	spref.get("start_modem_index", i, start_modem_index); start_modem_index = i;
	spref.get("end_modem_index",   i, end_modem_index);   end_modem_index   = i;

	memset(strbuff, 0, sizeof(strbuff));
	spref.get("start_modem_name", strbuff, start_modem_name.c_str(), sizeof(strbuff) - 1);
	start_modem_name = strbuff;

	memset(strbuff, 0, sizeof(strbuff));
	spref.get("end_modem_name", strbuff, end_modem_name.c_str(), sizeof(strbuff) - 1);
	end_modem_name = strbuff;

	spref.get("path0_active", i, path0_active); path0_active   = i;
	spref.get("path0_spread", f, path0_spread); path0_spread   = f;
	spref.get("path0_offset", f, path0_offset); path0_offset   = f;

	spref.get("path1_active", i, path0_active); path1_active   = i;
	spref.get("path1_delay",  f, path0_spread); path1_delay    = f;
	spref.get("path1_spread", f, path0_spread); path1_spread   = f;
	spref.get("path1_offset", f, path0_offset); path1_offset   = f;

	spref.get("path2_active", i, path0_active); path2_active   = i;
	spref.get("path2_delay",  f, path0_spread); path2_delay    = f;
	spref.get("path2_spread", f, path0_spread); path2_spread   = f;
	spref.get("path2_offset", f, path0_offset); path2_offset   = f;

	spref.get("awgn_active", i, awgn_active); awgn_active   = i;
	spref.get("awgn_snr",    f, awgn_snr);    awgn_snr      = f;

	spref.get("awgn_series_upper", i, awgn_series_upper); awgn_series_upper = i;
	spref.get("awgn_series_lower", i, awgn_series_lower); awgn_series_lower = i;
	spref.get("awgn_series_step",  i, awgn_series_step);  awgn_series_step  = i;

	memset(strbuff, 0, sizeof(strbuff));
	spref.get("batch_directory", strbuff, batch_directory.c_str(), sizeof(strbuff) - 1);
	batch_directory = strbuff;

	memset(strbuff, 0, sizeof(strbuff));
	spref.get("models", strbuff, models.c_str(), sizeof(strbuff) - 1);
	models = strbuff;

	memset(strbuff, 0, sizeof(strbuff));
	spref.get("simulations", strbuff, simulations.c_str(), sizeof(strbuff) - 1);
	models = strbuff;

	spref.get("sample_rate_menu_index", i, sample_rate_menu_index); sample_rate_menu_index = i;

	spref.get("tx_duration", i, tx_duration); tx_duration = i;
}

void set_float(Fl_Float_Input *widget, float value)
{
	char __buffer__[128];
	memset(__buffer__, 0, sizeof(__buffer__));
	snprintf(__buffer__, sizeof(__buffer__)-1, "%1.2f", value);
	widget->value(__buffer__);
}

void set_integer(Fl_Input *widget, int value)
{
	char __buffer__[128];
	memset(__buffer__, 0, sizeof(__buffer__));
	snprintf(__buffer__, sizeof(__buffer__) - 1, "%d", value);
	widget->value(__buffer__);
}

void status::initLastState(void)
{
	int index = 0;

	if(populate_modem_list()) {
		index = start_modem_choice->find_index(start_modem_name.c_str());
		if(index > -1)
			start_modem_index = index;
		else
			start_modem_index = 0;

		index = end_modem_choice->find_index(end_modem_name.c_str());
		if(index > -1)
			end_modem_index = index;
		else
			end_modem_index = 0;

		if(start_modem_index > end_modem_index) {
			int temp = start_modem_index;
			start_modem_index = end_modem_index;
			end_modem_index = temp;
		}

		start_modem_choice->value(start_modem_index);
		end_modem_choice->value(end_modem_index);
	}

	if(ck_high_speed) ck_high_speed->value(high_speed);

	if(cntr_High_dB) cntr_High_dB->value(progStatus.awgn_series_upper);
	if(cntr_Low_dB)  cntr_Low_dB->value(progStatus.awgn_series_lower);
	if(cntr_Step_dB) cntr_Step_dB->value(progStatus.awgn_series_step);

	if(p0_on)       p0_on->value(progStatus.path0_active);
	if(inp_spread0) set_float(inp_spread0, progStatus.path0_spread);
	if(inp_offset0) set_float(inp_offset0, progStatus.path0_offset);

	if(p1_on)       p1_on->value(progStatus.path1_active);
	if(inp_delay1)  set_float(inp_delay1,  progStatus.path1_delay);
	if(inp_spread1) set_float(inp_spread1, progStatus.path1_spread);
	if(inp_offset1) set_float(inp_offset1, progStatus.path1_offset);

	if(p2_on)       p2_on->value(progStatus.path2_active);
	if(inp_delay2)  set_float(inp_delay2,  progStatus.path2_delay);
	if(inp_spread2) set_float(inp_spread2, progStatus.path2_spread);
	if(inp_offset2) set_float(inp_offset2, progStatus.path2_offset);

	if(inp_AWGN_on)  inp_AWGN_on->value(progStatus.awgn_active);
	if(inp_AWGN_rms) set_float(inp_AWGN_rms, progStatus.awgn_snr);

	//if(txt_simulations_filename) txt_simulations_filename->value(progStatus.models.c_str());

}
