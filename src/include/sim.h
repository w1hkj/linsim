// ---------------------------------------------------------------------
// SIM.h: interface for the SIM class.
//
// ---------------------------------------------------------------------
// Copyright 2000, Moe Wheatley AE4JY <ae4jy@mindspring.com>
// Modified  2013, Dave Freese W1HKJ  <w1hkj@w1hkj.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms o7f the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// ---------------------------------------------------------------------

#ifndef SIH
#define SIH

#include "noise_gen.h"
#include "delay.h"
#include "path.h"
#include "cmplx.h"
#include "sound.h"

#define MAX_BUF_SIZE 2048	// maximum # of data values to process at a time
#define RMS_MAXAMPLITUDE (4000.0 / 32768.0)
//#define RMS_MAXAMPLITUDE 4000.0	//RMS amplitude out of 32768
								// pick so that worst case Gaussian noise
								// plus signals will not overflow soundcard
#define RMSAVE 20

struct PATH_INFO {
	bool active;
	double spread;
	double offset;
	int blocksize;
};

struct DELAY_INFO {
	bool direct;
	double delay1;
	double delay2;
};


class SIM {
public:
	SIM();
	~SIM();
	void Process( double *samples, int BUF_SIZE);
	void measure_rms( double *samples, int BUF_SIZE);

	void SetsnrValue(double snrdb) {
		snr = pow(10.0, snrdb/20.0);
	}
	void ReadGains( double& noise_gain, double& signal_gain) {
		signal_gain = signal_gain;
		noise_gain = noise_rms / RMS_MAXAMPLITUDE;
	}

	void AWGN( bool val) { b_awgn = val; }
	bool AWGN() { return b_awgn; }

	double signal_rms;
	double noise_rms;

	void init(double sr, PATH_INFO &p0, PATH_INFO &p1, PATH_INFO &p2, DELAY_INFO &d);

	SoundFile sound_in;
	SoundFile sound_out;

private:
	bool b_awgn;
	int    numpaths;
	double samplerate;
	double signal_gain;
	double snr;
	double ssum;
	double sim_buffer[MAX_BUF_SIZE];
	cmplx delay0_buffer[MAX_BUF_SIZE];
	cmplx delay1_buffer[MAX_BUF_SIZE];
	cmplx delay2_buffer[MAX_BUF_SIZE];
	CDelay* delay;
	CPath* path0;
	CPath* path1;
	CPath* path2;
	CNoiseGen* noise_gen;
};

#endif
