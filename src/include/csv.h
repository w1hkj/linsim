// =====================================================================
//
// csvdb.h
//
// comma separated value database file support for linsim
//
// Copyright 2013, Dave Freese, W1HKJ
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
// =====================================================================

#ifndef CSVDB_H
#define CSVDB_H

#include <stdlib.h>

#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

struct csvRecord {
	std::string title;
	std::string awgn;
	std::string sn;
	std::string p0;
	std::string spread_0;
	std::string offset_0;
	std::string p1;
	std::string delay_1;
	std::string spread_1;
	std::string offset_1;
	std::string p2;
	std::string delay_2;
	std::string spread_2;
	std::string offset_2;
	csvRecord() {
		title.clear();
		awgn.clear();
		sn.clear();
		p0.clear();
		spread_0.clear();
		offset_0.clear();
		p1.clear();
		delay_1.clear();
		spread_1.clear();
		offset_1.clear();
		p2.clear();
		delay_2.clear();
		spread_2.clear();
		offset_2.clear();
	}
	csvRecord(std::string atitle, std::string wgn, std::string asn, std::string ap0, std::string asp0, std::string aof0,
			std::string ap1, std::string adel1, std::string asp1, std::string aof1,
			std::string ap2, std::string adel2, std::string asp2, std::string aof2){
		title = atitle;
		awgn = wgn;
		sn = asn;
		p0 = ap0;
		spread_0 = asp0;
		offset_0 = aof0;
		p1 = ap1;
		delay_1 = adel1;
		spread_1 = asp1;
		offset_1 = aof1;
		p2 = ap2;
		delay_2 = adel2;
		spread_2 = asp2;
		offset_2 = aof2;
	}
};

class csvdb {
private:
	static const char *csvFields;
	std::string dbfilename;
	csvRecord record;
	int cur_recnbr;
public:
	std::vector<csvRecord> dbrecs;

	csvdb() {
		dbfilename.clear();
		cur_recnbr = 0;
	}
	csvdb(std::string fname) {
		dbfilename.assign(fname);
		cur_recnbr = 0;
	}
	~csvdb() {}
	void filename(std::string fname) { dbfilename.assign(fname); }
	std::string filename() { return dbfilename; }
	int save();
	int load();
	void load_defaults();
	int numrecs() { return (int)dbrecs.size(); }
	int get(size_t n, csvRecord &rec);
	int put(size_t n, csvRecord &rec);
	int add(csvRecord &rec);
	void sort();
	int recnbr() { return cur_recnbr; }
	int erase(size_t n);
	std::string delimit(std::string s);
	std::string trim(std::string s);
	void field(std::string &s, std::string &fld);
	bool split(std::string s, csvRecord &rec);
	void join(csvRecord &rec, std::string &s);
	void clearrec(csvRecord &rec);
	bool mapheader(std::string s);
	void printrec(int);
};

#endif
