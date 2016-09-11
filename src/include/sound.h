// ----------------------------------------------------------------------------
//
//      sound.h
//
// Copyright (C) 2006-2013
//              Dave Freese, W1HKJ
//              Robert Stiles, KK5VD
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

#include <sndfile.h>
#include <samplerate.h>

#define SND_BUFFER_DATA_TYPE      float
#define PROCESS_BUFFER_DATA_TYPE  double

#define	SND_FRAME_LEN        65536
#define SND_UNIT_LEN         (sizeof(SND_BUFFER_DATA_TYPE) * SND_FRAME_LEN)
#define DEFAULT_SAMPLE_RATE  8000
#define RW_FRAME_COUNT       1024
#define LEFT_CHANNEL         0
#define RIGHT_CHANNEL        1

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

class SoundFile {
public:
	enum {READ, WRITE};

protected:
	SNDFILE* snd_file;
	int		format;
	int		sample_frequency;
	int		mode;
	int     no_of_channels;

	size_t read_frame_count;
	size_t read_unit_count;
	size_t write_frame_count;
	size_t write_unit_count;

	SF_INFO read_info;
	SF_INFO write_info;

	SRC_STATE	*writ_src_state;
	SRC_STATE	*read_src_state;

	SRC_DATA	*writ_src_data;
	SRC_DATA	*read_src_data;

	size_t src_io_buffer_units;
	SND_BUFFER_DATA_TYPE * src_out_buffer;
	SND_BUFFER_DATA_TYPE * src_inp_buffer;
	SND_BUFFER_DATA_TYPE * inp_pointer;

	std::string fname;

	void	 tag_file(SNDFILE *sndfile, const char *title);

public:
	SoundFile(std::string _fname = "", int _mode = READ, int freq = 8000);
	~SoundFile();
	bool    init_src(int);
	void    free_src(void);
	inline SND_BUFFER_DATA_TYPE * inc_frame(SND_BUFFER_DATA_TYPE *frame_pointer);
	inline SND_BUFFER_DATA_TYPE * inc_frame(SND_BUFFER_DATA_TYPE *frame_pointer, size_t frames);

	int     open(std::string _fname, int _mode);
	void    close();

	size_t  write(SND_BUFFER_DATA_TYPE *buf, size_t count);
	size_t  read(SND_BUFFER_DATA_TYPE *buf, size_t count);

	size_t  write(PROCESS_BUFFER_DATA_TYPE *buf, size_t count);
	size_t  read(PROCESS_BUFFER_DATA_TYPE *buf, size_t count);

	void rewind();
	size_t size();

	int io_channels(void) { return no_of_channels; }
	//	size_t  Write_stereo(double *, double *, size_t);

};

extern std::string file_name(std::string def_fname);
extern int inpfile_samplerate;
extern int outfile_samplerate;

#endif // SOUND_H
