// ---------------------------------------------------------------------
// SIM.cpp: implementation of the SIM class.
//
// ---------------------------------------------------------------------
// Copyright 2000, Moe Wheatley AE4JY <ae4jy@mindspring.com>
// Modified  2013, Dave Freese W1HKJ  <w1hkj@w1hkj.com>
// Modified  2016, Robert Stiles      <kk5vd@yahoo.com>
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
// ---------------------------------------------------------------------

#include "sim.h"

#include <stdio.h>
#include <string>

#include <iostream>

SIM::SIM()
{
	delay     = (CDelay *)0;
	path0     = (CPath *)0;
	path1     = (CPath *)0;
	path2     = (CPath *)0;
	noise_gen = (CNoiseGen *)0;

	reset();
}

SIM::~SIM()
{
	if(noise_gen)
		delete noise_gen;
	if(delay)
		delete delay;
	if(path0)
		delete path0;
	if(path1)
		delete path1;
	if(path2)
		delete path2;
}

//----------------------------------------------------------------------
// Initialize the simulator parameters
// Called before any Processing
//----------------------------------------------------------------------
void SIM::reset(void)
{
	if(noise_gen)
		delete noise_gen;
	noise_gen = new CNoiseGen;

	if(delay)
		delete delay;
	delay = new CDelay;

	if(path0)
		delete path0;
	path0 = new CPath;

	if(path1)
		delete path1;
	path1 = new CPath;

	if(path2)
		delete path2;
	path2 = new CPath;

	b_awgn      = false;
	numpaths    = 0;
	samplerate  = 8000;
	ssum        = 0.0;
	signal_rms  = 0.0;
	noise_rms   = 0.0;
	signal_gain = 1.0;
	num_buffs   = 0;
	signal_peak = 0;

	size_t max_buffer_size = MAX_FRAME_COUNT * 2;

	memset(sim_buffer,    0, sizeof(sim_buffer));
	memset(delay0_buffer, 0, sizeof(delay0_buffer));
	memset(delay1_buffer, 0, sizeof(delay1_buffer));
	memset(delay2_buffer, 0, sizeof(delay2_buffer));
}

//----------------------------------------------------------------------
// Initialize the simulator parameters
// Called before any Processing
//----------------------------------------------------------------------
void SIM::init(double sr, PATH_INFO &p0, PATH_INFO &p1, PATH_INFO &p2, DELAY_INFO &d)
{
	samplerate = sr;

	noise_gen->InitNoiseGen();

	if(p0.active) numpaths++;
	if(p1.active) numpaths++;
	if(p2.active) numpaths++;

	path0->SampleRate(sr);
	path0->InitPath( p0.spread, p0.offset, max_buffer_size, numpaths, p0.active );
	path1->SampleRate(sr);
	path1->InitPath( p1.spread, p1.offset, max_buffer_size, numpaths, p1.active );
	path2->SampleRate(sr);
	path2->InitPath( p2.spread, p2.offset, max_buffer_size, numpaths, p2.active );
	delay->SampleRate(sr);
	delay->SetDelays( d.delay1, d.delay2 );

}

// ---------------------------------------------------------------------
// Measure RMS of input wav file
// ---------------------------------------------------------------------
void SIM::measure_rms( PROCESS_BUFFER_DATA_TYPE *samples, int BUF_SIZE)
{
	//	ssum = 0.0;
	double smpl, peak;
	int step = sound_in.io_channels();

	for( int i = 0; i < BUF_SIZE; i += step) {
		smpl = samples[i];
		peak = fabs(smpl);
		signal_rms += (smpl * smpl);
		//		ssum += ( smpl * smpl );
		if (peak > signal_peak) signal_peak = peak;
	}
	//	signal_rms += ssum;
	num_buffs += (BUF_SIZE / step);
	//	signal_rms = (signal_rms * num_buffs + sqrtf(ssum));
	//	num_buffs += BUF_SIZE;
	//	signal_rms /= num_buffs;

}

// ---------------------------------------------------------------------
//  Process the real input data stream in BLOCKSIZE increments
// ---------------------------------------------------------------------
void SIM::Process(PROCESS_BUFFER_DATA_TYPE *samples, int BUF_SIZE)
{

	size_t step = sound_in.io_channels();
	size_t j = 0;
	size_t i = 0;
	size_t frames = BUF_SIZE / step;

	path0->blocksize(frames);
	path1->blocksize(frames);
	path2->blocksize(frames);

	j = 0;
	for(i = 0; i < frames; i++, j += step) {
		sim_buffer[i] = samples[j];
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
		for (int i = 0; i < frames; i++)
			sim_buffer[i] = (delay0_buffer[i].x + delay1_buffer[i].x + delay2_buffer[i].x);
	}

	// Add bandwidth limited noise
	if (b_awgn) {
		noise_gen->AddBWLimitedNoise(frames, sim_buffer,
									 signal_gain,
									 noise_rms * sqrtf(samplerate / 8000.0) );
	}

	switch(step) {
		case 1:
			for (i = 0; i < frames; i++)
				samples[i] = sim_buffer[i];
			break;
			
		case 2:
			j = 0;
			for (i = 0; i < frames; i++) {
				samples[j++] = sim_buffer[i];
				samples[j++] = 0.0;
			}
			break;
	}
}

