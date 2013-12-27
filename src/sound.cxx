// ----------------------------------------------------------------------------
//
//      sound.cxx
//
// Copyright (C) 2006-2013
//              Dave Freese, W1HKJ
//
// Copyright (C) 2007-2013
//              Stelios Bounanos, M0GLD
//
// This file is part of linSIM.
//
// linSIM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// linSIM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with linSIM.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include <FL/Fl_File_Chooser.H>

#include "sound.h"

#define	SND_BUF_LEN      65536
#define SNDFILE_CHANNELS 1

using namespace std;

//----------------------------------------------------------------------
// support for picking a filename
//----------------------------------------------------------------------

std::string file_name(std::string def_fname)
{
	char *picked = fl_file_chooser	( "Audio File", "*.wav", def_fname.c_str(), 1);
	if (!picked) return "";
	return picked;
}

//----------------------------------------------------------------------
// Class SoundFile implementation
//----------------------------------------------------------------------

SoundFile::SoundFile(std::string _fname, int _mode, int freq)
        : snd_file(0),
          format(SF_FORMAT_WAV | SF_FORMAT_PCM_16)
{
	fname = _fname;
	mode = _mode;
	sample_frequency = freq;
	if (!fname.empty()) open(fname, mode);
}

SoundFile::~SoundFile()
{
	if (mode == WRITE) sf_write_sync(snd_file) ;
	close();
}

int SoundFile::open(std::string _fname, int _mode) 
{
	fname = _fname;
	mode = _mode;
	if (mode == WRITE) {
		SF_INFO info = { 0, sample_frequency, SNDFILE_CHANNELS, format, 0, 0 };
		if ((snd_file = sf_open(fname.c_str(), SFM_WRITE, &info)) == NULL) {
			fprintf(stderr, "Could not write %s:%s", fname.c_str(), sf_strerror(NULL) );
			return 0;
		}
		if (sf_command(snd_file, SFC_SET_UPDATE_HEADER_AUTO, NULL, SF_TRUE) != SF_TRUE)
			fprintf(stderr, "sound file update header command failed: %s", sf_strerror(snd_file));
			tag_file(snd_file, "linsim output");
	} else {
		if (fname.empty())
			return -1;
		SF_INFO info = { 0, 0, 0, 0, 0, 0 };
		if ((snd_file = sf_open(fname.c_str(), SFM_READ, &info)) == NULL) {
			fprintf(stderr, "Could not read %s:%s", fname.c_str(), sf_strerror(NULL) );
			return -2;
		}
		if (info.samplerate != 8000) {
			sf_close(snd_file);
			snd_file = NULL;
			return -3;
		}
		if (info.channels != 1) {
			sf_close(snd_file);
			snd_file = NULL;
			return -4;
		}
	}
	return 0; 
}

void SoundFile::close()
{
	if (!snd_file) return;
	sf_close(snd_file);
	snd_file = NULL;
}

void SoundFile::rewind()
{
	sf_seek(snd_file, 0, SEEK_SET);
}

size_t SoundFile::read(float* buf, size_t count)
{
	if (!snd_file || mode != READ) return 0;

	size_t r = sf_read_float(snd_file, buf, count);
	if (r == 0) return 0;

	size_t rd = 0;
	while (r < count) {
		rd = sf_read_float(snd_file, buf + r, count - r);
		if (rd == 0) break;
		r += rd;
	}
	return r;
}

size_t SoundFile::read(double* buf, size_t count)
{
	if (!snd_file || mode != READ) return 0;

	size_t r = sf_read_double(snd_file, buf, count);
	if (r == 0) return 0;

	size_t rd = 0;
	while (r < (size_t)count) {
		sf_seek(snd_file, 0, SEEK_CUR);
		rd = sf_read_double(snd_file, buf + r, count - r);
		if (rd == 0) break;
		r += rd;
	}
	return r;
}

size_t SoundFile::write(float* buf, size_t count)
{
	if (!snd_file || mode != WRITE) return count;
	return (sf_write_float(snd_file, buf, count));
}

size_t SoundFile::write(double* buf, size_t count)
{
	if (!snd_file || mode != WRITE) return count;
	return (sf_write_double(snd_file, buf, count));
}

void SoundFile::tag_file(SNDFILE *sndfile, const char *title)
{
	int err;
	if ((err = sf_set_string(sndfile, SF_STR_TITLE, title)) != 0) {
		fprintf(stderr, "sf_set_string STR_TITLE: %s", sf_error_number(err));
		return;
	}

	sf_set_string(sndfile, SF_STR_COPYRIGHT, "W1HKJ");
	sf_set_string(sndfile, SF_STR_SOFTWARE,  "linSIM-1.0.0");
	sf_set_string(sndfile, SF_STR_ARTIST,    "W1HKJ");
	sf_set_string(sndfile, SF_STR_COMMENT,   "linSIM test file");

	char s[200];
	time_t t = time(0);
	struct tm *zt;
	zt = gmtime(&t);
	if (strftime(s, sizeof(s), "%Y-%m-%dT%H:%M:%Sz", zt) > 0)
		sf_set_string(sndfile, SF_STR_DATE, s);
}
