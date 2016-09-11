//======================================================================
// flmsg xmlrpc_client.cxx
//
// Copyright (C) 2012, 2016 Dave Freese W1HKJ, Robert Stiles KK5VD
//
// This file is part of LinSim.
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
// xmlrpc interface to fldigi
//
// fetches current list of modem types from fldigi
// fetches current modem in use in fldigi
// sets fldigi modem-by-name when required
//
//======================================================================

#include <stdio.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <queue>

#include <iostream>
#include <errno.h>

#include "xmlrpc_client.h"
#include "xmlrpcpp/XmlRpc.h"
#include "xmlrpcpp/XmlRpcValue.h"
#include "threads.h"
#include "configure.h"
#include "debug.h"

using namespace std;
using namespace XmlRpc;

#define DEFAULT_XMLRPC_TIMEOUT 6.0

double xmlrpc_timeout = DEFAULT_XMLRPC_TIMEOUT;

pthread_mutex_t mutex_xmlrpc = PTHREAD_MUTEX_INITIALIZER;
pthread_t *xmlrpc_thread = (pthread_t *) 0;

bool xmlrpc_thread_running = false;
bool xmlrpc_thread_exit    = false;
bool xmlrpc_thread_exiting = false;

// these are get only
static const char * modem_get_name        = "modem.get_name";
static const char * modem_get_names       = "modem.get_names";

// these are set only
static const char * modem_set_by_name     = "modem.set_by_name";
static const char * modem_highspeed       = "modem.highspeed";

static const char * text_clear_tx         = "text.clear_tx";
static const char * text_add_tx           = "text.add_tx";
static const char * text_get_rx           = "text.get_rx";
static const char * rx_get_data           = "rx.get_data";
static const char * add_tx_queue          = "text.add_tx_queu";

static const char * main_get_trx_state    = "main.get_trx_state";
static const char * main_tx               = "main.tx";
static const char * main_rx               = "main.rx";
static const char * main_abort            = "main.abort";
static const char * main_get_char_rates   = "main.get_char_rates";
static const char * main_get_tx_timing    = "main.get_tx_timing";
static const char * main_get_char_timing  = "main.get_char_timing";

static const char * audio_play_file       = "audio.playback";
static const char * audio_record_file     = "audio.record";
static const char * audio_record_stop     = "audio.record_stop";
static const char * audio_play_rec_status = "audio.status";

static const char * get_linsim_rx_data_str = "linsim.get_rx_data";
static const char * set_record_data_str    = "linsim.record_rx_data";
static const char * set_read_count_str     = "linsim.set_read_count";
static const char * get_linsim_status_str  = "linsim.status";

static XmlRpc::XmlRpcClient * client = (XmlRpc::XmlRpcClient *) 0;

#define XMLRPC_UPDATE_INTERVAL    200
#define XMLRPC_UPDATE_AFTER_WRITE 1000
#define XMLRPC_RETRY_INTERVAL     2000

extern int errno;
int xmlrpc_errno = 0;
std::string fldigi_modes;
bool fldigi_online = false;
bool logerr = true;

/*****************************************************************
 * socket ops
 *****************************************************************/
int update_interval = XMLRPC_UPDATE_INTERVAL;

string xmlcall = "";

/*****************************************************************/
void open_xmlrpc()
{
	if(client) return;

	pthread_mutex_lock(&mutex_xmlrpc);

	client = new XmlRpc::XmlRpcClient( progStatus.xmlrpc_ip_address.c_str(), progStatus.xmlrpc_ip_port_no );

	pthread_mutex_unlock(&mutex_xmlrpc);

	//	XmlRpc::setVerbosity(5); // 0...5
}

/*****************************************************************/
void close_xmlrpc()
{
	pthread_mutex_lock(&mutex_xmlrpc);

	delete client;
	client = NULL;

	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
static inline void execute(const char* name, const XmlRpcValue& param, XmlRpcValue& result)
{
	if (client) {
		if (!client->execute(name, param, result, xmlrpc_timeout)) {
			xmlrpc_errno = errno;

			if(client->isFault())
				LOG_DEBUG("Server fault response!");

			throw XmlRpc::XmlRpcException(name);
		}
	}
	xmlrpc_errno = errno;
}

/*****************************************************************/
void set_xmlrpc_timeout(double value)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	if(value < DEFAULT_XMLRPC_TIMEOUT) return;
	xmlrpc_timeout = value;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void set_xmlrpc_timeout_default(void)
{
	xmlrpc_timeout = DEFAULT_XMLRPC_TIMEOUT;
}

/*****************************************************************
 * send functions
 *****************************************************************/
bool send_modem_highspeed(bool state)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpcValue res;
		XmlRpcValue data(state);
		execute(modem_highspeed, data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************
 * send functions
 *****************************************************************/
bool set_record_data(bool state)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		XmlRpc::XmlRpcValue data(state);
		execute(set_record_data_str, data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************
 * send functions
 *****************************************************************/
bool set_read_count(int count)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		XmlRpc::XmlRpcValue data(count);
		execute(set_read_count_str, data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}


/*****************************************************************/
bool send_add_tx_queue(std::string &data)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		XmlRpc::XmlRpcValue _data(data.c_str());
		execute(add_tx_queue, _data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_audio_play_file(std::string &filename_path)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		XmlRpc::XmlRpcValue data(filename_path.c_str());
		execute(audio_play_file, data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_audio_record_file(std::string &filename_path)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		XmlRpc::XmlRpcValue data(filename_path.c_str());
		execute(audio_record_file, data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_audio_record_stop(void)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		execute(audio_record_stop, 0, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_new_modem(std::string &__modem)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue mode(__modem.c_str()), res;
		execute(modem_set_by_name, mode, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_clear_tx(void)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		execute(text_clear_tx, 0, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_report(std::string &report)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		XmlRpc::XmlRpcValue data = report.c_str();
		execute(text_clear_tx, 0, res);
		execute(text_add_tx, data, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_tx(void)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		execute(main_tx, 0, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_rx(void)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		execute(main_rx, 0, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
bool send_abort(void)
{
	pthread_mutex_lock(&mutex_xmlrpc);
	bool error = false;
	try {
		XmlRpc::XmlRpcValue res;
		execute(main_abort, 0, res);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		error = true;
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
	return error;
}

/*****************************************************************/
void get_tx_timing(std::string &data, std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue xmlData((void *)data.c_str(), data.size());

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(main_get_tx_timing, xmlData, status);
		string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE * 4;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void get_char_timing(int character, std::string &response)
{
	pthread_mutex_lock(&mutex_xmlrpc);

	XmlRpc::XmlRpcValue status;
	char buff[20];

	memset(buff, 0, sizeof(buff));

	snprintf(buff, sizeof(buff) - 1, "%d", character);

	std::string data;
	data.assign(buff);

	XmlRpcValue xmlData((void *) data.c_str(), data.size());

	try {
		execute(main_get_char_timing, xmlData, status);
		std::string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}

	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************
 * receive functions
 *****************************************************************/
static void set_combo(void *str)
{

}

/*****************************************************************/
void get_linsim_rx_data(std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue query;
	int size = 0;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(get_linsim_rx_data_str, query, status);
		XmlRpcValue::BinaryData resp(size);
		resp = status;
		size = status.size();
		for(int i = 0; i < size; i++)
			response += resp[i];

	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE * 2;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void get_linsim_status(std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue query;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(get_linsim_status_str, query, status);
		string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE * 2;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void get_audio_play_rec_status(std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue query;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(audio_play_rec_status, query, status);
		std::string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
}


/*****************************************************************/
void get_trx_state(std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue query;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(main_get_trx_state, query, status);
		std::string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void get_char_rates(std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue query;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(main_get_char_rates, query, status);
		std::string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void get_fldigi_modem(std::string &response)
{
	XmlRpc::XmlRpcValue status;
	XmlRpc::XmlRpcValue query;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		execute(modem_get_name, query, status);
		string resp = status;
		response.assign(resp);
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
		response.clear();
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
static char *invalid_modem_list[] = {
	(char *) "NULL",
	(char *) "IFKP",
	(char *) "FSQ",
	(char *) "FELDHELL",
	(char *) "SLOWHELL",
	(char *) "HELLX5",
	(char *) "HELLX9",
	(char *) "FSKHELL",
	(char *) "FSKH105",
	(char *) "HELL80",
	(char *) "WEFAX576",
	(char *) "WEFAX288",
	(char *) "SSB",
	(char *) "WWV",
	(char *) "ANALYSIS",
	(char *) "NAVTEX",
	(char *) "SITORB",
	(char *) "FREQSCAN"
};

/*****************************************************************/
static inline bool invalid_modem(std::string &modem)
{
	int count = sizeof(invalid_modem_list) / sizeof(char *);
	for(int index = 0; index < count; index++)
		if(modem == invalid_modem_list[index]) return true;
	return false;
}

/*****************************************************************/
void get_fldigi_modems(std::string &modems)
{
	XmlRpc::XmlRpcValue status, query;
	std::string modem;

	pthread_mutex_lock(&mutex_xmlrpc);
	try {
		fldigi_modes.clear();
		execute(modem_get_names, query, status);
		for (int i = 0; i < status.size(); i++) {
			modem.assign((std::string) status[i]);
			if(!invalid_modem(modem))
				fldigi_modes.append(modem).append("|");
		}
		modems.assign(fldigi_modes);
		fldigi_online = true;
		logerr = true;
	} catch (const XmlRpc::XmlRpcException& e) {
		LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
	}
	update_interval = XMLRPC_UPDATE_AFTER_WRITE;
	pthread_mutex_unlock(&mutex_xmlrpc);
}

/*****************************************************************/
void * xmlrpc_loop(void *d)
{
	xmlrpc_thread_running = true;
	std::string dummy;

	fldigi_online = false;
	for (;;) {
		try {
			if(fldigi_online)
				get_fldigi_modem(dummy);
			else
				get_fldigi_modems(dummy);
		} catch (const XmlRpc::XmlRpcException& e) {
			if (logerr) {
				LOG_ERROR("%s xmlrpc_errno = %d", e.getMessage().c_str(), xmlrpc_errno);
				logerr = false;
			}

			pthread_mutex_lock(&mutex_xmlrpc);
			fldigi_online = false;
			update_interval = XMLRPC_RETRY_INTERVAL;
			pthread_mutex_unlock(&mutex_xmlrpc);

		}

		MilliSleep(update_interval);

		pthread_mutex_lock(&mutex_xmlrpc);
		if (update_interval != XMLRPC_UPDATE_INTERVAL)
			update_interval = XMLRPC_UPDATE_INTERVAL;
		pthread_mutex_unlock(&mutex_xmlrpc);

		if(xmlrpc_thread_exit) break;
	}

	xmlrpc_thread_exiting = true;
	return NULL;
}

/** ********************************************************
 *  test for running fldigi made immediately after starting
 *  the xmlrpc thread
 ***********************************************************/
bool test_for_fldigi(void)
{
	std::string test;
	get_trx_state(test);
	if (test == "NIL" || test.empty()) {
		return false;
	}
	return true;
}

/*****************************************************************/
void terminate_xmlrpc(void)
{
	if(xmlrpc_thread_running) {
		xmlrpc_thread_exit = true;

		if(xmlrpc_thread) {
			pthread_join(*xmlrpc_thread, 0);
			delete xmlrpc_thread;
			xmlrpc_thread = (pthread_t *) 0;
		}
		
		xmlrpc_thread_running = false;
		xmlrpc_thread_exit    = false;
		xmlrpc_thread_exiting = false;
	}
	
	if(client)
		close_xmlrpc();
}

/*****************************************************************/
void start_xmlrpc(void)
{
	if(xmlrpc_thread_running) return;
	
	xmlrpc_thread_running = false;
	xmlrpc_thread_exit    = false;
	xmlrpc_thread_exiting = false;
	
	open_xmlrpc();
	xmlrpc_thread = new pthread_t;
	if (pthread_create(xmlrpc_thread, NULL, xmlrpc_loop, NULL)) {
		perror("pthread_create: xmlrpc");
		terminate_xmlrpc();
	}
}


