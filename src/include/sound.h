// ----------------------------------------------------------------------------
//
//      sound.h
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

#ifndef _SOUND_H
#define _SOUND_H

#include <string>
#include <cstring>
#include <climits>

#  include <sndfile.h>
/*
class SndException : public std::exception
{
public:
	SndException(int err_ = 0)
		: err(err_), msg(std::string("Sound error: ") + err_to_str(err_))
	{ }
	SndException(const char* msg_)
		: err(1), msg(msg_)
	{ }
	SndException(int err_, const std::string& msg_) : err(err_), msg(msg_) { }
	virtual ~SndException() throw() { }

	const char*	what(void) const throw() { return msg.c_str(); }
	int		error(void) const { return err; }

protected:
	const char* err_to_str(int e) { return strerror(e); }

	int		err;
	std::string	msg;
};
*/

#define SNDFILE_CHANNELS 1

class SoundFile {
public:
enum {READ, WRITE};

protected:
	int		sample_frequency;
	int		format;
	int		mode;

	SNDFILE* snd_file;
//	float    *wrt_buffer;

	std::string fname;

	void	 tag_file(SNDFILE *sndfile, const char *title);

public:
	SoundFile(std::string _fname = "", int _mode = READ, int freq = 8000);
	~SoundFile();
	int     open(std::string _fname, int _mode);
	void    close();

	size_t  write(float *buf, size_t count);
	size_t  read(float *, size_t);

	size_t  write(double *buf, size_t count);
	size_t  read(double *, size_t);

	void rewind();

//	size_t  Write_stereo(double *, double *, size_t);

};

extern std::string file_name(std::string def_fname);

#endif // SOUND_H
