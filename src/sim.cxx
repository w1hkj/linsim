// ---------------------------------------------------------------------
// SIM.cpp: implementation of the SIM class.
//
// ---------------------------------------------------------------------
// Copyright 2000, Moe Wheatley AE4JY <ae4jy@mindspring.com>
// Modified  2013, Dave Freese W1HKJ  <w1hkj@w1hkj.com>
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
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// ---------------------------------------------------------------------

#include "sim.h"

#include <stdio.h>
#include <string>

SIM::SIM()
{
	noise_gen = new CNoiseGen;
	delay = new CDelay;
	path0 = new CPath;
	path1 = new CPath;
	path2 = new CPath;

	ssum = 0.0;
	snr = 1.0;
	signal_rms = 0.0;
	signal_gain = 1.0;
	b_awgn = false;
	samplerate = 8000;
}

SIM::~SIM()
{
	delete noise_gen;
	delete delay;
	delete path0;
	delete path1;
	delete path2;
}

//----------------------------------------------------------------------
// Initialize the simulator parameters
// Called before any Processing
//----------------------------------------------------------------------

void SIM::init(double sr, PATH_INFO &p0, PATH_INFO &p1, PATH_INFO &p2, DELAY_INFO &d)
{
	numpaths = 0;

	samplerate = sr;
	signal_rms = RMS_MAXAMPLITUDE;

	noise_gen->InitNoiseGen();

	if(p0.active) numpaths++;
	if(p1.active) numpaths++;
	if(p2.active) numpaths++;

	path0->SampleRate(sr);
	path0->InitPath( p0.spread, p0.offset, MAX_BUF_SIZE, numpaths, p0.active );
	path1->SampleRate(sr);
	path1->InitPath( p1.spread, p1.offset, MAX_BUF_SIZE, numpaths, p1.active );
	path2->SampleRate(sr);
	path2->InitPath( p2.spread, p2.offset, MAX_BUF_SIZE, numpaths, p2.active );
	delay->SampleRate(sr);
	delay->SetDelays( d.delay1, d.delay2 );

}

// ---------------------------------------------------------------------
// Measure RMS of input wav file
// ---------------------------------------------------------------------
void SIM::measure_rms( double *samples, int BUF_SIZE)
{
	ssum = 0.0;
	double smpl;
	for( int i = 0; i <BUF_SIZE; i++) {
		smpl = samples[i];
		ssum += ( smpl * smpl );
		if (fabs(smpl) > signal_peak) signal_peak = fabs(smpl);
	}
	ssum /= BUF_SIZE;
	num_buffs++;
	signal_rms = (signal_rms * (num_buffs - 1) + ssum) / num_buffs;
}

// ---------------------------------------------------------------------
//  Process the real input data stream in BLOCKSIZE increments
// ---------------------------------------------------------------------
void SIM::Process( double *samples, int BUF_SIZE)
{
	path0->blocksize(BUF_SIZE);
	path1->blocksize(BUF_SIZE);
	path2->blocksize(BUF_SIZE);

	for( int i = 0; i < BUF_SIZE; i++) {
		sim_buffer[i] = samples[i];
		delay0_buffer[i] = delay1_buffer[i] = delay2_buffer[i] = cmplx(0,0);
	}

	if (numpaths) {
//Bandpass filter into I and Q and get delayed versions of the input data
		delay->CalcBPFilter(sim_buffer, delay0_buffer);
		delay->CreateDelays(
					delay0_buffer,
					path1->active(), delay1_buffer, 
					path2->active(), delay2_buffer);

//Calculate each path.
		path0->CalcPath( delay0_buffer, delay0_buffer );
		path1->CalcPath( delay1_buffer, delay1_buffer );
		path2->CalcPath( delay2_buffer, delay2_buffer );

// Sum and Copy just the real part back into the real buffer for output
		for (int i = 0; i < BUF_SIZE; i++)
			sim_buffer[i] = (delay0_buffer[i].x + delay1_buffer[i].x + delay2_buffer[i].x);
	}

// Add bandwidth limited noise
	if (b_awgn) {
		noise_gen->AddBWLimitedNoise(BUF_SIZE, sim_buffer, 
			signal_gain, noise_rms * sqrtf(samplerate / 8000) );
//			samplerate == 48000 ? noise_rms * 10 :
//			noise_rms );
	}

	for (int i = 0; i < BUF_SIZE; i++)
		samples[i] = sim_buffer[i];
}

