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

SIM::SIM()
{
	fft = new Cfft;
	path0 = new CPath;
	path1 = new CPath;
	path2 = new CPath;
	delay = new CDelay;
	noise_gen = new CNoiseGen;

	ssum = 0.0;
	snr = 1.0;
	signal_rms = 0.0;
	signal_gain = 1.0;
	b_awgn = false;
	samplerate = 8000;

}

SIM::~SIM()
{
	if (noise_gen) delete noise_gen;
	if (delay) delete delay;
	if (path0) delete path0;
	if (path1) delete path1;
	if (path2) delete path2;
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
	fft->ResetFFT();

	if(p0.active) numpaths++;
	if(p1.active) numpaths++;
	if(p2.active) numpaths++;

	if (numpaths) {
		path0->InitPath( p0.active, p0.spread, p0.offset, p0.blocksize, samplerate, numpaths );
		path1->InitPath( p1.active, p1.spread, p1.offset, p1.blocksize, samplerate, numpaths );
		path2->InitPath( p2.active, p2.spread, p2.offset, p2.blocksize, samplerate, numpaths );
		delay->SetDelays( d.delay1, d.delay2, samplerate );
	}

}

// ---------------------------------------------------------------------
// Measure RMS of input wav file
// ---------------------------------------------------------------------
void SIM::measure_rms( double *samples, int BUF_SIZE)
{
	ssum = 0.0;
	for( int i = 0; i <BUF_SIZE; i++)
		ssum += ( samples[i]*samples[i] );
	ssum /= BUF_SIZE;

//Simple IIR LP filter the rms averages
	signal_rms = (1.0 / RMSAVE) * sqrt(ssum) + (1.0 - 1.0 / RMSAVE)*signal_rms;
}

// ---------------------------------------------------------------------
//  Process the real input data stream in BLOCKSIZE increments
// ---------------------------------------------------------------------
void SIM::Process( double *samples, int BUF_SIZE)
{
	for( int i = 0; i <BUF_SIZE; i++)
		sim_buffer[i] = samples[i];

	if (numpaths) {
//Bandpass filter into I and Q and get delayed versions of the input data
		delay->calc_bpf(sim_buffer, delay0_buffer );
		delay->CreateDelays(delay0_buffer, delay1_buffer, delay2_buffer);

//Calculate each path.
		path0->CalcPath( delay0_buffer, delay0_buffer );
		path1->CalcPath( delay1_buffer, delay1_buffer );
		path2->CalcPath( delay2_buffer, delay2_buffer );

// Sum and Copy just the real part back into the real buffer for output
		for (int i = 0; i < BUF_SIZE; i++)
			sim_buffer[i] = (delay0_buffer[i].re + delay1_buffer[i].re + delay2_buffer[i].re) / numpaths; 
	}

// Add bandwidth limited noise
	if (!b_awgn) {
		signal_gain = 1.0;
		noise_rms = 0.0;
	} else {
		if (snr >= 1.0) {
			signal_gain = RMS_MAXAMPLITUDE / signal_rms;
			noise_rms = (signal_gain * signal_rms) / snr;
			noise_gen->AddBWLimitedNoise(BUF_SIZE, sim_buffer, signal_gain, noise_rms);
		} else {
			signal_gain = (RMS_MAXAMPLITUDE*snr) / signal_rms;
			noise_rms = RMS_MAXAMPLITUDE;
			noise_gen->AddBWLimitedNoise(BUF_SIZE, sim_buffer, signal_gain, RMS_MAXAMPLITUDE);
		}
	}

	for (int i = 0; i < BUF_SIZE; i++)
		samples[i] = sim_buffer[i];
}

