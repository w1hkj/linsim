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

using namespace std;

struct csvRecord {
	string title;
	string awgn;
	string sn;
	string p0;
	string spread_0;
	string offset_0;
	string p1;
	string delay_1;
	string spread_1;
	string offset_1;
	string p2;
	string delay_2;
	string spread_2;
	string offset_2;
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
	csvRecord(string atitle, string wgn, string asn, string ap0, string asp0, string aof0,
			string ap1, string adel1, string asp1, string aof1,
			string ap2, string adel2, string asp2, string aof2){
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
	string dbfilename;
	csvRecord record;
	int cur_recnbr;
public:
	vector<csvRecord> dbrecs;

	csvdb() {
		dbfilename.clear();
		cur_recnbr = 0;
	}
	csvdb(string fname) {
		dbfilename.assign(fname);
		cur_recnbr = 0;
	}
	~csvdb() {}
	void filename(string fname) { dbfilename.assign(fname); }
	string filename() { return dbfilename; }
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
	string delimit(string s);
	string trim(string s);
	void field(string &s, string &fld);
	bool split(string s, csvRecord &rec);
	void join(csvRecord &rec, string &s);
	void clearrec(csvRecord &rec);
	bool mapheader(string s);
	void printrec(int);
};

#endif
