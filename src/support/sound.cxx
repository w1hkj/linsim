// ----------------------------------------------------------------------------
//
//      sound.cxx
//
// Copyright (C) 2006-2016
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
#include <FL/filename.H>

#include "config.h"
#include "sound.h"
#include "main.h"
#include "linsim_ui.h"
#include "util.h"

using namespace std;

int inpfile_samplerate = DEFAULT_SAMPLE_RATE;
int outfile_samplerate = DEFAULT_SAMPLE_RATE;
int linsim_samplerate  = DEFAULT_SAMPLE_RATE;


SF_INFO wav_file_info;

extern std::string batch_dir;
extern bool modem_series_flag;

//----------------------------------------------------------------------
// support for picking a filename
//----------------------------------------------------------------------

std::string file_name(std::string def_fname)
{
	def_fname.insert(0, BaseDir);
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
	memset(&read_info,  0, sizeof(read_info));
	memset(&write_info, 0, sizeof(write_info));

	writ_src_state = (SRC_STATE *) 0;
	read_src_state = (SRC_STATE *) 0;

	writ_src_data  = (SRC_DATA *) 0;
	read_src_data  = (SRC_DATA *) 0;

	src_out_buffer = (SND_BUFFER_DATA_TYPE *) 0;
	src_inp_buffer = (SND_BUFFER_DATA_TYPE *) 0;
	inp_pointer    = (SND_BUFFER_DATA_TYPE *) 0;

	src_io_buffer_units = 0;
	no_of_channels      = 1;

	read_unit_count     = RW_FRAME_COUNT * no_of_channels;
	read_frame_count    = RW_FRAME_COUNT;

	write_unit_count    = RW_FRAME_COUNT * no_of_channels;
	write_frame_count   = RW_FRAME_COUNT;

	std::string fname = "";

	fname = _fname;
	mode  = _mode;

	linsim_samplerate = sample_frequency = freq;

	if (!fname.empty())
		open(fname, mode);
}

/***********************************************************************
 *
 ***********************************************************************/
SoundFile::~SoundFile()
{
	if (mode == WRITE)
		sf_write_sync(snd_file) ;
	close();
	free_src();
}

/***********************************************************************
 *
 ***********************************************************************/
bool SoundFile::init_src(int nchs)
{
	int err = 0;
	bool flag = true;

	free_src();

	read_unit_count  = RW_FRAME_COUNT * nchs;
	read_frame_count = RW_FRAME_COUNT;

	read_src_data = new SRC_DATA;

	read_src_state = src_new(SRC_SINC_MEDIUM_QUALITY, nchs, &err);
	if (read_src_state == 0) {
		throw SndException(src_strerror(err));
		return false;
	}

	write_unit_count  = RW_FRAME_COUNT * nchs;
	write_frame_count = RW_FRAME_COUNT;

	writ_src_data = new SRC_DATA;

	writ_src_state = src_new(SRC_SINC_MEDIUM_QUALITY, nchs, &err);
	if (writ_src_state == 0) {
		throw SndException(src_strerror(err));
		return false;
	}

	src_io_buffer_units = SND_FRAME_LEN * nchs;

	src_inp_buffer = new SND_BUFFER_DATA_TYPE [src_io_buffer_units];
	if (!src_inp_buffer) return false;

	memset(src_inp_buffer, 0, src_io_buffer_units * sizeof(SND_BUFFER_DATA_TYPE));

	src_out_buffer = new SND_BUFFER_DATA_TYPE [src_io_buffer_units];
	if (!src_out_buffer) return false;

	memset(src_out_buffer, 0, src_io_buffer_units * sizeof(SND_BUFFER_DATA_TYPE));

	inp_pointer = src_out_buffer;

	return true;
}

/***********************************************************************
 *
 ***********************************************************************/
void SoundFile::free_src(void)
{
	int err = 0;
	bool return_flag = true;

	if(writ_src_data) {
		delete writ_src_data;
		writ_src_data = (SRC_DATA *) 0;
	}

	if(writ_src_state) {
		src_delete(writ_src_state);
		writ_src_state = (SRC_STATE *) 0;
	}

	if(read_src_data) {
		delete read_src_data;
		read_src_data = (SRC_DATA *) 0;
	}

	if(read_src_state) {
		src_delete(read_src_state);
		read_src_state = (SRC_STATE *) 0;
	}

	if(src_out_buffer) {
		delete [] src_out_buffer;
		src_out_buffer = (SND_BUFFER_DATA_TYPE *) 0;
	}

	if(src_inp_buffer) {
		delete [] src_inp_buffer;
		src_inp_buffer = (SND_BUFFER_DATA_TYPE *) 0;
	}
}

/***********************************************************************
 * Increment SND_BUFFER_DATA_TYPE pointer by one frame.
 ***********************************************************************/
inline SND_BUFFER_DATA_TYPE *
SoundFile::inc_frame(SND_BUFFER_DATA_TYPE *frame_pointer)
{
	if(frame_pointer && no_of_channels)
		return &frame_pointer[no_of_channels];
	else
		return frame_pointer;
}

/***********************************************************************
 * Increment SND_BUFFER_DATA_TYPE pointer by a number of frames.
 ***********************************************************************/
inline SND_BUFFER_DATA_TYPE *
SoundFile::inc_frame(SND_BUFFER_DATA_TYPE *frame_pointer, size_t frames)
{
	if(frame_pointer && no_of_channels)
		return &frame_pointer[no_of_channels * frames];
	else
		return frame_pointer;
}

/***********************************************************************
 *
 ***********************************************************************/
int SoundFile::open(std::string _fname, int _mode)
{
	std::string local_fn;
	no_of_channels = 0;

	fname = _fname;

	if(!modem_series_flag) {
		local_fn = _fname;
	} else {
		local_fn.assign(batch_dir).append(_fname);
	}

	mode = _mode;

	if (mode == WRITE) {
		write_info.frames = 0 ;
		write_info.samplerate = outfile_samplerate;
		write_info.channels   = wav_file_info.channels;
		write_info.format     = wav_file_info.format;
		write_info.sections   = 0;
		write_info.seekable   = 0;

		no_of_channels = write_info.channels;

		/*
		 printf("WRITE\nframes : %d\nsamplerate : %d\nchannels : %d\nformat: %d\nsections: %d\nseekable: %d\n",
		 static_cast<unsigned int>(write_info.frames),
		 write_info.samplerate, write_info.channels, write_info.format,
		 write_info.sections, write_info.seekable);
		 */

		if ((snd_file = sf_open(local_fn.c_str(), SFM_WRITE, &write_info)) == NULL) {
			fprintf(stderr, "Could not write %s:%s", fname.c_str(), sf_strerror(NULL) );
			return 0;
		}

		if(!init_src(write_info.channels)) {
			free_src();
			return -6;
		}

		if (sf_command(snd_file, SFC_SET_UPDATE_HEADER_AUTO, NULL, SF_TRUE) != SF_TRUE)
			fprintf(stderr, "sound file update header command failed: %s", sf_strerror(snd_file));

		tag_file(snd_file, "linsim output");

		std::string wav_comment, wav_title;
		size_t pc;
		wav_title.assign(fl_filename_name(fname_in.c_str()));
		pc = wav_title.find(".wav");
		if (pc != std::string::npos) wav_title.erase(pc);

		wav_comment.assign(txt_simulation->value());
		pc = wav_comment.find(".wav");
		if (pc != std::string::npos) wav_comment.erase(pc);

		sf_set_string (snd_file, SF_STR_SOFTWARE, PACKAGE_STRING);
		sf_set_string (snd_file, SF_STR_ARTIST, "W1HKJ software");
		sf_set_string (snd_file, SF_STR_TITLE, wav_title.c_str());
		sf_set_string (snd_file, SF_STR_COMMENT, wav_comment.c_str() );

		writ_src_data->src_ratio = 1.0 * outfile_samplerate / linsim_samplerate;
		src_set_ratio(writ_src_state, writ_src_data->src_ratio);

	} else {

		if (local_fn.empty())
			return -1;

		read_info.frames = 0 ;
		read_info.samplerate = 0;
		read_info.channels = 0;
		read_info.format = 0;
		read_info.sections = 0;
		read_info.seekable = 0;

		if ((snd_file = sf_open(local_fn.c_str(), SFM_READ, &read_info)) == NULL) {
			fprintf(stderr, "Could not read %s:%s", fname.c_str(), sf_strerror(NULL) );
			return -2;
		}

		if (read_info.channels < 1 || read_info.channels > 2) {
			sf_close(snd_file);
			snd_file = NULL;
			return -4;
		} else {
			no_of_channels = read_info.channels;

			if(!init_src(read_info.channels)) {
				free_src();
				return -6;
			}
		}

		/*
		 printf("READ\nframes : %d\nsamplerate : %d\nchannels : %d\nformat: %d\nsections: %d\nseekable: %d\n",
		 static_cast<unsigned int>(read_info.frames),
		 read_info.samplerate, read_info.channels, read_info.format,
		 read_info.sections, read_info.seekable);
		 */

		wav_file_info = read_info;

		file_sample_rate  = inpfile_samplerate = read_info.samplerate;
		linsim_samplerate = inpfile_samplerate;

		read_src_data->src_ratio = 1.0 * linsim_samplerate / inpfile_samplerate;
		src_set_ratio(read_src_state, read_src_data->src_ratio);
		memset(src_inp_buffer, 0, src_io_buffer_units);

		//printf("read ratio %f\n", read_src_data->src_ratio);

	}
	return 0;
}

/***********************************************************************
 *
 ***********************************************************************/
void SoundFile::close()
{
	if (!snd_file) return;
	sf_close(snd_file);
	snd_file = NULL;
}

/***********************************************************************
 * Return the number of samples (frames) in the audio file.
 ***********************************************************************/
size_t SoundFile::size()
{
	size_t flen = sf_seek(snd_file, 0, SEEK_END);
	rewind();
	return flen;
}

/***********************************************************************
 *	Reset the file pointer to the start of the audio samples (frames)
 ***********************************************************************/
void SoundFile::rewind()
{
	sf_seek(snd_file, 0, SEEK_SET);
}

/***********************************************************************
 *
 ***********************************************************************/
void SoundFile::tag_file(SNDFILE *sndfile, const char *title)
{
	int err;
	if ((err = sf_set_string(sndfile, SF_STR_TITLE, title)) != 0) {
		fprintf(stderr, "sf_set_string STR_TITLE: %s", sf_error_number(err));
		return;
	}

	sf_set_string(sndfile, SF_STR_COPYRIGHT, "W1HKJ");
	sf_set_string(sndfile, SF_STR_SOFTWARE,  "linSIM");
	sf_set_string(sndfile, SF_STR_ARTIST,    "W1HKJ");
	sf_set_string(sndfile, SF_STR_COMMENT,   "linSIM test file");

	char s[200];
	time_t t = time(0);
	struct tm *zt;
	zt = gmtime(&t);
	if (strftime(s, sizeof(s), "%Y-%m-%dT%H:%M:%Sz", zt) > 0)
		sf_set_string(sndfile, SF_STR_DATE, s);
}

// ---------------------------------------------------------------------
// read_file
// source samplerate is arbitrary, linsim samplerate is 8000
// read from file and resample until a "count" number of converted samples
// is available, or until at the end of the input file
// ---------------------------------------------------------------------
size_t SoundFile::read(SND_BUFFER_DATA_TYPE * buf, size_t frames)
{
	sf_count_t r_frames = 0, rd_frame_count = 0;
	int err = 0;
	size_t units = frames * no_of_channels;

	memset(src_inp_buffer, 0, src_io_buffer_units * sizeof(SND_BUFFER_DATA_TYPE));

	while ( static_cast<size_t>(inp_pointer - src_out_buffer) < units) {
		rd_frame_count = sf_readf_float(snd_file, src_inp_buffer, RW_FRAME_COUNT);
		if (!rd_frame_count) break;

		read_src_data->data_in      = src_inp_buffer;
		read_src_data->input_frames = rd_frame_count;
		read_src_data->data_out     = inp_pointer;

		long _frames = (src_io_buffer_units - (inp_pointer - src_out_buffer)) / no_of_channels;
		if( _frames < 1) _frames = 0;

		read_src_data->output_frames = _frames;
		read_src_data->end_of_input = 0;

		if ((err = src_process(read_src_state, read_src_data)) != 0)
			throw SndException(src_strerror(err));

		inp_pointer = inc_frame(inp_pointer, read_src_data->output_frames_gen);
	}

	if ( static_cast<size_t>(inp_pointer - src_out_buffer) >= units) {
		memcpy(buf, src_out_buffer, units * sizeof(SND_BUFFER_DATA_TYPE));
		memmove(src_out_buffer, &src_out_buffer[units], (src_io_buffer_units - units) * sizeof(SND_BUFFER_DATA_TYPE));
		inp_pointer -= units;
		r_frames = units / no_of_channels;
	}

	if (r_frames == 0) {
		src_reset (read_src_state);
		inp_pointer = src_out_buffer;
	}

	return (size_t) r_frames;
}

/***********************************************************************
 *
 ***********************************************************************/
size_t SoundFile::read(PROCESS_BUFFER_DATA_TYPE * buf, size_t units)
{
	Allocate<SND_BUFFER_DATA_TYPE> B;
	SND_BUFFER_DATA_TYPE *buffer = B.allocate(units);

	if(!buffer) return 0;

	int frames = units / no_of_channels;

	sf_count_t read_frames = read(buffer, frames);

	size_t read_units = read_frames * no_of_channels;

	for(size_t i = 0; i < read_units; i++)
		buf[i] = buffer[i];

	return (size_t) read_frames;
}

// ---------------------------------------------------------------------
// write_file
//----------------------------------------------------------------------
size_t SoundFile::write(SND_BUFFER_DATA_TYPE *buf, size_t frames)
{
	int err;

	writ_src_data->data_in       = buf;
	writ_src_data->input_frames  = frames;
	writ_src_data->data_out      = src_out_buffer;
	writ_src_data->output_frames = SND_FRAME_LEN;
	writ_src_data->end_of_input  = 0;

	if ((err = src_process(writ_src_state, writ_src_data)) != 0) {
		throw SndException(src_strerror(err));
	}

	size_t output_size = writ_src_data->output_frames_gen;

	if (output_size)
		sf_writef_float(snd_file, writ_src_data->data_out, output_size);

	return output_size;
}

/***********************************************************************
 *
 ***********************************************************************/
size_t SoundFile::write(PROCESS_BUFFER_DATA_TYPE *buf, size_t units)
{
	size_t frames = units / no_of_channels;
	size_t i = 0;
	size_t j = 0;
	
	Allocate<SND_BUFFER_DATA_TYPE> B;
	float *outbuf = B.allocate(units);
	if(!outbuf) return 0;
	
	switch(no_of_channels) {
		case 1:
			for(i = 0; i < units; i++)
				outbuf[i] = buf[i];
			break;
			
		case 2:
			j = 0;
			for(i = 0; i < units; i += no_of_channels) {
				outbuf[j++] = buf[i]; // Copy left channel
				outbuf[j++] = 0.0;    // Zero right channel
			}
			break;
	}
	
	size_t output_size = write(outbuf, frames);
	
	return output_size;
}

