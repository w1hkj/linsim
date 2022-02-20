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
#include <FL/filename.H>

#include "config.h"
#include "sound.h"
#include "main.h"
#include "linsim_ui.h"

#define	SND_BUF_LEN      65536
#define SNDFILE_CHANNELS 1
#define SND_BUF_LEN	 65536
#define SND_RW_LEN	(8 * SND_BUF_LEN)

int inpfile_samplerate = 8000;
int outfile_samplerate = 8000;
int linsim_samplerate = 8000;
SF_INFO wav_file_info;

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
	fname = _fname;
	mode = _mode;
	linsim_samplerate = sample_frequency = freq;
	if (!fname.empty()) open(fname, mode);

	int err;
	writ_src_data = new SRC_DATA;
	read_src_data = new SRC_DATA;

	writ_src_state = src_new(SRC_SINC_MEDIUM_QUALITY, 1, &err);
	if (writ_src_state == 0)
		throw SndException(src_strerror(err));

	read_src_state = src_new(SRC_SINC_MEDIUM_QUALITY, 1, &err);
	if (read_src_state == 0)
		throw SndException(src_strerror(err));

	if (read_src_state == 0)
		throw SndException(src_strerror(err));
	src_out_buffer = new float [SND_RW_LEN];
	if (!src_out_buffer)
		throw SndException(src_strerror(err));
	src_inp_buffer = new float [SND_RW_LEN];
	if (!src_inp_buffer)
		throw SndException(src_strerror(err));
	inp_pointer = src_out_buffer;
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
		write_info.frames = 0 ;
		write_info.samplerate = outfile_samplerate;
		write_info.channels = wav_file_info.channels;
		write_info.format = wav_file_info.format;
		write_info.sections = 0;
		write_info.seekable = 0;
/*
printf("WRITE\nframes : %d\nsamplerate : %d\nchannels : %d\nformat: %d\nsections: %d\nseekable: %d\n", 
	static_cast<unsigned int>(write_info.frames),
	write_info.samplerate, write_info.channels, write_info.format,
	write_info.sections, write_info.seekable);
*/
		if ((snd_file = sf_open(fname.c_str(), SFM_WRITE, &write_info)) == NULL) {
			fprintf(stderr, "Could not write %s:%s", fname.c_str(), sf_strerror(NULL) );
			return 0;
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
		if (fname.empty())
			return -1;
		read_info.frames = 0 ;
		read_info.samplerate = 0;
		read_info.channels = 0;
		read_info.format = 0;
		read_info.sections = 0;
		read_info.seekable = 0;
		if ((snd_file = sf_open(fname.c_str(), SFM_READ, &read_info)) == NULL) {
			fprintf(stderr, "Could not read %s:%s", fname.c_str(), sf_strerror(NULL) );
			return -2;
		}
		if (read_info.channels != 1) {
			sf_close(snd_file);
			snd_file = NULL;
			return -4;
		}
/*
printf("READ\nframes : %d\nsamplerate : %d\nchannels : %d\nformat: %d\nsections: %d\nseekable: %d\n", 
static_cast<unsigned int>(read_info.frames),
read_info.samplerate, read_info.channels, read_info.format,
read_info.sections, read_info.seekable);
*/
		wav_file_info = read_info;

		inpfile_samplerate = read_info.samplerate;

		linsim_samplerate = inpfile_samplerate;

		read_src_data->src_ratio = 1.0 * linsim_samplerate / inpfile_samplerate;
		src_set_ratio(read_src_state, read_src_data->src_ratio);
		memset(src_inp_buffer, 0, 1024 * sizeof(float));

//printf("read ratio %f\n", read_src_data->src_ratio);

	}
	return 0; 
}

void SoundFile::close()
{
	if (!snd_file) return;
	sf_close(snd_file);
	snd_file = NULL;
}

size_t SoundFile::size()
{
	size_t flen = sf_seek(snd_file, 0, SEEK_END);
	rewind();
	return flen;
}

void SoundFile::rewind()
{
	sf_seek(snd_file, 0, SEEK_SET);
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

// ---------------------------------------------------------------------
// read_file
// source samplerate is arbitrary, linsim samplerate is 8000
// read from file and resample until a "count" number of converted samples
// is available, or until at the end of the input file
// ---------------------------------------------------------------------
size_t SoundFile::read(float* buf, size_t count)
{
	sf_count_t r = 0, rd_count = 0;
	int err = 0;

	memset(src_inp_buffer, 0, 1024 * sizeof(float));

	while ( static_cast<size_t>(inp_pointer - src_out_buffer) < count) {
		rd_count = sf_readf_float(snd_file, src_inp_buffer, 1024);
		if (!rd_count) break;
		read_src_data->data_in = src_inp_buffer;
		read_src_data->input_frames = rd_count;
		read_src_data->data_out = inp_pointer;
		read_src_data->output_frames = SND_RW_LEN - (inp_pointer - src_out_buffer);
		read_src_data->end_of_input = 0;

		if ((err = src_process(read_src_state, read_src_data)) != 0)
			throw SndException(src_strerror(err));

		inp_pointer += read_src_data->output_frames_gen;
	}

	if ( static_cast<size_t>(inp_pointer - src_out_buffer) >= count) {
		memcpy(buf, src_out_buffer, count * sizeof(float));
		memmove(src_out_buffer, src_out_buffer + count, (SND_RW_LEN - count) * sizeof(float));
		inp_pointer -= count;
		r = count;
	}

	if (r == 0) {
		src_reset (read_src_state);
		inp_pointer = src_out_buffer;
	}
	return (size_t)r;
}

size_t SoundFile::read(double* buf, size_t count)
{
	float buffer[count];
	sf_count_t r =read(buffer, count);
	for (sf_count_t i = 0; i < r; i++) buf[i] = buffer[i];
	return (size_t)r;
}

// ---------------------------------------------------------------------
// write_file
//----------------------------------------------------------------------

size_t SoundFile::write(float* buf, size_t count)
{
	int err;

	writ_src_data->data_in = buf;
	writ_src_data->input_frames = count;
	writ_src_data->data_out = src_out_buffer;
	writ_src_data->output_frames = SND_RW_LEN;
	writ_src_data->end_of_input = 0;

	if ((err = src_process(writ_src_state, writ_src_data)) != 0) {
		throw SndException(src_strerror(err));
	}

	size_t output_size = writ_src_data->output_frames_gen;

	if (output_size)
		sf_writef_float(snd_file, writ_src_data->data_out, output_size);

	return output_size;

}

size_t SoundFile::write(double* buf, size_t count)
{
	float *outbuf = new float[count];
	for (size_t i = 0; i < count; i++)
		outbuf[i] = buf[i];
	size_t output_size = write(outbuf, count);
	delete [] outbuf;
	return output_size;
}

