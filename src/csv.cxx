// =====================================================================
//
// csvdb.cxx
//
// comma separated value database file support for linsim
//
// Copyright 2013, Dave Freese, W1HKJ
//
// =====================================================================

#include "csv.h"

extern string HomeDir;

csvRecord defaults[] = {
csvRecord("CCIR 520-2 (Doppler Fading)",
"0",  "0",  "1","0.2", "10",  "1","0.5","0.2",  "5",  "0",  "0",  "0",  "0"),
csvRecord("CCIR 520-2 (Flutter Fading)",
"0",  "0",  "1",  "1", "10",  "1","0.5", "10", "10",  "0",  "0",  "0",  "0"),
csvRecord("CCIR 520-2 (Good Conditions)",
"0",  "0",  "1",  "1","0.1",  "1","0.5","0.1","0.1",  "0",  "0",  "0",  "0"),
csvRecord("CCIR-520-2 (Moderate Conditions)",
"0",  "0",  "1",  "1","0.5",  "1",  "1","0.5","0.5",  "0",  "0",  "0",  "0"),
csvRecord("CCIR-520-2 (Poor Conditions)",
"0",  "0",  "1",  "1","0.5",  "1",  "2","0.5","0.5",  "0",  "0",  "0",  "0"),
csvRecord("Direct Path",
"1", "10",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0"),
csvRecord("Frequency Shifter",
"0",  "0",  "1",  "0","500",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0"),
csvRecord("High-Latitude Disturbed",
"0",  "0",  "1",  "1", "30",  "1",  "7", "30", "30",  "0",  "0",  "0",  "0"),
csvRecord("High-Latitude Moderate",
"0",  "0",  "1",  "1", "10",  "1",  "3", "10", "10",  "0",  "0",  "0",  "0"),
csvRecord("High-Latitude Quiet",
"0",  "0",  "1",  "1","0.5",  "1",  "1","0.5","0.5",  "0",  "0",  "0",  "0"),
csvRecord("Mid-Latitude Disturbed",
"0",  "0",  "1",  "1",  "1",  "1",  "2",  "1",  "1",  "0",  "0",  "0",  "0"),
csvRecord("Mid-Latitude Disturbed NVIS",
"0",  "0",  "1",  "1",  "1",  "1",  "7",  "1",  "1",  "0",  "0",  "0",  "0"),
csvRecord("Mid-Latitude Moderate",
"0",  "0",  "1",  "1","0.5",  "1",  "1","0.5","0.5",  "0",  "0",  "0",  "0"),
csvRecord("Mid-Latitude Quiet",
"0",  "0",  "1",  "1","0.5",  "1","0.5","0.1","0.1",  "0",  "0",  "0",  "0"),
csvRecord("Low-Latitude Disturbed",
"0",  "0",  "1",  "1", "10",  "1",  "6", "10", "10",  "0",  "0",  "0",  "0"),
csvRecord("Low-Latitude Moderate",
"0",  "0",  "1",  "1","1.5",  "1",  "2","1.5","1.5",  "0",  "0",  "0",  "0"),
csvRecord("Low-Latitude Quiet",
"0",  "0",  "1",  "1","0.5",  "1","0.5","0.5","0.5",  "0",  "0",  "0",  "0") };

void csvdb::load_defaults()
{
	csvRecord rec;

	dbrecs.clear();

	for (size_t n = 0; n < sizeof(defaults) / sizeof(csvRecord); n++)
		dbrecs.push_back(defaults[n]);
}

enum FIELDS {
TITLE, AWGN, SN,
P1, SPREAD_1, OFFSET_1,
P2, DELAY_2, SPREAD_2, OFFSET_2,
P3, DELAY_3, SPREAD_3, OFFSET_3
};

static int field_position[OFFSET_3];

//szFields on 10 char spacing
static string szFields = "\
TITLE     AWGN      S/N       \
P1        SPREAD_1  OFFSET_1  \
P2        DELAY_2   SPREAD_2  OFFSET_2  \
P3        DELAY_3   SPREAD_3  OFFSET_3  ";

const char *csvdb::csvFields = "\
TITLE,AWGN,S/N,P1,SPREAD_1,OFFSET_1,P2,DELAY_2,SPREAD_2,OFFSET_2,P3,DELAY_3,SPREAD_3,OFFSET_3";

bool csvdb::mapheader(string header)
{
	bool ok = false;
	size_t p;
	string hfield;
	for (int i = 0; i < OFFSET_3; field_position[i++] = -1);
	for (int i = 0; i < OFFSET_3; i++) {
		if (header.empty()) break;
		field( header, hfield);
		p = szFields.find(hfield);
		if (p != string::npos) {
			field_position[i] = p/10;
			ok = true;
		}
	}
	return ok;
}

//----------------------------------------------------------------------
// split separates a comma delimited string into the record components
// string may be simple comma separated, quoted-comma delimited or a
// mix of the two.  The csv file may be opened by a 3rd party program
// such as gnumeric or Excel and then resaved as a csv file.
// field may appear as:
// field,    first field unquoted
// ,field    last field unquoted
// ,"field a b c",
// ,"field ""a"" b c",
// ,"field a, b, c",
// "field",  first field quoted
// ,"field"   last field quoted
//
// field extracts the leading field in the referenced string
// the leading field is removed from the string
//----------------------------------------------------------------------
void csvdb::field(string &s, string &fld)
{
	// empty reference string ==> empty field
	if (s.empty()) {
		fld.clear();
		return;
	}
	size_t p;
	// remove leading comma
	if (s[0] == ',') {
		s.erase(0, 1);
	}
	if (s[0] == '"') {
	// field is quoted
		s.erase(0, 1);
		p = s.find("\","); // find end of field
		if (p == string::npos) {
			p = s.find("\""); // possibly last record
			if ( p != string::npos) {
				fld = s.substr(0, p);
				s.clear();
				return;
			} else {
				s.clear(); // error in reference line
				fld.clear();
				return;
			}
		}
		if (p == 0) {
			fld.clear();
			s.erase(0, 1);
		} else {
			fld = s.substr(0, p);
			s.erase(0, p+1);
		}
	} else {
	// field is only comma delimited
		if (s[0] == ',') {
			fld.clear();
		} else {
			p = s.find(",");
			if (p == string::npos) {
				fld = s;
				s.clear();
			} else {
				fld = s.substr(0, p);
				s.erase(0, p);
			}
		}
	}
	// change double quotes to single
	while ((p = fld.find("\"\"")) != string::npos)
		fld.erase(p, 1);
}

bool csvdb::split(string s, csvRecord &rec)
{
	clearrec(rec);

	if (s[0] == ',')
		s.insert(0,","); // first field is empty !

	for (int i = 0; i < OFFSET_3; i++) {
		switch (field_position[i]) {
			case -1 : break;
			case TITLE :		field(s, rec.title); break;
			case AWGN :			field(s, rec.awgn); break;
			case SN :			field(s, rec.sn); break;
			case P1 :			field(s, rec.p0); break;
			case SPREAD_1 :		field(s, rec.spread_0); break;
			case OFFSET_1 :		field(s, rec.offset_0); break;
			case P2 :			field(s, rec.p1); break;
			case DELAY_2 :		field(s, rec.delay_1); break;
			case SPREAD_2 :		field(s, rec.spread_1); break;
			case OFFSET_2 :		field(s, rec.offset_1); break;
			case P3 :			field(s, rec.p2); break;
			case DELAY_3 :		field(s, rec.delay_2); break;
			case SPREAD_3 :		field(s, rec.spread_2); break;
			case OFFSET_3 :		field(s, rec.offset_2); break;
		}
	}
	if (rec.awgn.empty() && rec.p0.empty() && rec.p1.empty() && rec.p2.empty())
		return false;
	if (rec.title.empty())
		rec.title.assign("unnamed");
	return true;
}

int csvdb::load()
{
#define LINESIZE 1024
	char buff[LINESIZE + 1];
	string pathname = HomeDir;
	pathname.append(dbfilename);
	fstream dbfile(pathname.c_str(), ios::in | ios::binary);
	if (!dbfile) {
		load_defaults();
		return 0;
	}

	csvRecord rec;

	dbrecs.clear();

// read & map header line
	memset(buff, 0, LINESIZE + 1);
	dbfile.getline(buff, LINESIZE);
	if (!mapheader(buff)) return -1;

// header passes test, read rest of file
	string sbuff;
	while (dbfile) {
		memset(buff, 0, LINESIZE + 1);
		dbfile.getline(buff, LINESIZE);
		if (dbfile && strlen(buff)) {
			sbuff = buff;
			if (split(buff, rec)) dbrecs.push_back(rec);
		}
	}
	dbfile.close();
	return 0;
}

string csvdb::trim(string s)
{
	string trimmed;
	trimmed.assign(s);
	while (trimmed.length() && trimmed[0] == ' ') trimmed.erase(0);
	while (trimmed.length() && trimmed[trimmed.length()-1] == ' ')
		trimmed.erase(trimmed.length() - 1);
	return trimmed;
}

string csvdb::delimit(string s)
{
	static string delimited;
	bool quoted = false;
	delimited.assign(trim(s));
	size_t p = delimited.find("\"");
	while (p != string::npos) {
		delimited.insert(p, "\"");
		quoted = true;
		p = delimited.find("\"", p+2);
	}
	if (delimited.find(" ") != string::npos ||
		delimited.find(",") != string::npos ||
		delimited.find(";") != string::npos ||
		delimited.find("'") != string::npos) quoted = true;
	if (quoted) {
		delimited.insert(0, "\"");
		delimited.append("\"");
	}
	return delimited;
}

static int compfunc(const void *r1, const void *r2) {
	csvRecord * p0 = (csvRecord *)r1;
	csvRecord * p1 = (csvRecord *)r2;
	return (p0->title).compare(p1->title);
}

//----------------------------------------------------------------------
// join combines the separate elements of a record into a comma
// separated string.  Quotes are used to delimit a field if that field
// contains an embedded comma or a ".  Quotes within a field are saved
// as a double quote.  All fields which contain white space including the
// quote, space, comma etc are quoted.  Trailing and leading spaces are
// removed to conserve file size
//----------------------------------------------------------------------
void csvdb::join(csvRecord &rec, string &str)
{
	str.assign(delimit(rec.title)).append(",");
	str.append(delimit(rec.awgn)).append(",");
	str.append(delimit(rec.sn)).append(",");
	str.append(delimit(rec.p0)).append(",");
	str.append(delimit(rec.spread_0)).append(",");
	str.append(delimit(rec.offset_0)).append(",");
	str.append(delimit(rec.p1)).append(",");
	str.append(delimit(rec.delay_1)).append(",");
	str.append(delimit(rec.spread_1)).append(",");
	str.append(delimit(rec.offset_1)).append(",");
	str.append(delimit(rec.p2)).append(",");
	str.append(delimit(rec.delay_2)).append(",");
	str.append(delimit(rec.spread_2)).append(",");
	str.append(delimit(rec.offset_2)).append(",");
}

void csvdb::sort()
{
	qsort ( &(dbrecs[0]), dbrecs.size(), sizeof(csvRecord), compfunc);
}

int csvdb::save()
{
	string pathname = HomeDir;
	pathname.append(dbfilename);
	fstream dbfile(pathname.c_str(), ios::out | ios::binary);
	if (!dbfile) return -1;

	sort();

// csv file header line
	dbfile << csvFields << "\n";
// records
	string line;
	for (size_t n = 0; n < dbrecs.size(); n++) {
		join(dbrecs[n], line);
		dbfile << line << "\n";
		line.clear();
	}

	dbfile.close();

	return 0;
}

void csvdb::clearrec(csvRecord &rec)
{
	rec.title.clear();
	rec.awgn.clear();
	rec.sn.clear();
	rec.p0.clear();
	rec.spread_0.clear();
	rec.offset_0.clear();
	rec.p1.clear();
	rec.delay_1.clear();
	rec.spread_1.clear();
	rec.offset_1.clear();
	rec.p2.clear();
	rec.delay_2.clear();
	rec.spread_2.clear();
	rec.offset_2.clear();
}

int csvdb::get(size_t n, csvRecord &rec)
{
	rec.title.clear();
	rec.awgn.clear();
	rec.sn.clear();
	rec.p0.clear();
	rec.spread_0.clear();
	rec.offset_0.clear();
	rec.p1.clear();
	rec.delay_1.clear();
	rec.spread_1.clear();
	rec.offset_1.clear();
	rec.p2.clear();
	rec.delay_2.clear();
	rec.spread_2.clear();
	rec.offset_2.clear();

	if (n < 0 || n >= dbrecs.size()) return -1;

	if (!dbrecs.size()) return -2;

	rec = dbrecs[n];

	cur_recnbr = n;

	return 0;
}


int csvdb::put(size_t recnbr, csvRecord &rec)
{
	dbrecs[recnbr] = rec;
	return 0;
}


int csvdb::add(csvRecord &rec)
{
	dbrecs.push_back(rec);
	return 0;
}


int csvdb::erase(size_t n)
{
	csvRecord rec;
	get(n, rec);
	vector<csvRecord>::iterator p = dbrecs.begin();
	while (p != dbrecs.end()) {
		if (p->title == rec.title) {
			dbrecs.erase(p);
			return 0;
		}
		p++;
	}
	return 1;
}

void csvdb::printrec(int n)
{
	string srec;
	join(dbrecs[n], srec);
	printf("%s\n", srec.c_str());
}
