// ---------------------------------------------------------------------
// Delay.cpp: implementation of the CDelay class.
//   ( also performs Hilbert Real to complex I/Q 3KHz filtering )
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
//
// ---------------------------------------------------------------------

#include "delay.h"
#include "filter_tables.h"

#define MAXDELAY (50 * 8)					// 50 mSecs max delay
#define BLOCKSIZE 2048
#define BUFSIZE (BLOCKSIZE + MAXDELAY)

// ---------------------------------------------------------------------
// Construction/Destruction
// ---------------------------------------------------------------------

CDelay::CDelay()
{
	pDelayLine = new complex[BUFSIZE];
	pQue = new complex[HILBPFIR_LENGTH];
	Inptr = 0;
	T1ptr = 0;
	T2ptr = 0;
}

CDelay::~CDelay()
{
	if(pDelayLine)
		delete pDelayLine;
	if(pQue)
		delete pQue;
}

// ---------------------------------------------------------------------
//  Set delay times
// ---------------------------------------------------------------------
void CDelay::SetDelays( double t1, double t2, double samplerate )	//delays in msecs
{
	for(int i = 0; i < HILBPFIR_LENGTH; i++) {
		pQue[i].re = 0.0;
		pQue[i].im = 0.0;
	}
	for(int i = 0; i < BUFSIZE; i++) {
		pDelayLine[i] = complex(0, 0);
	}
	Inptr = BUFSIZE - 1;
	FirState = HILBPFIR_LENGTH - 1;
	T1ptr = BUFSIZE - (int)(samplerate * t1 / 1000.0) - 1;
	T2ptr = BUFSIZE - (int)(samplerate * t2 / 1000.0) - 1;
}

// ---------------------------------------------------------------------
//  Uses pointers to create variable delays
// ---------------------------------------------------------------------
void CDelay::CreateDelays( complex* inbuf, complex* t1buf, complex* t2buf )
{
	for (int i = 0; i < BLOCKSIZE; i++) {
		pDelayLine[Inptr++] = inbuf[i]; // copy new data from inbuf into delay buffer
		if( Inptr >= BUFSIZE )
			Inptr = 0;
		t1buf[i] = pDelayLine[T1ptr++];
		if( T1ptr >= BUFSIZE )
			T1ptr = 0;
		t2buf[i] = pDelayLine[T2ptr++];
		if( T2ptr >= BUFSIZE )
			T2ptr = 0;
	}
}


// ---------------------------------------------------------------------
// Hilbert 3KHz BP filters.  Real input and complex I/Q output
//   This FIR bandwidth limits the real input as well as creates a
//   complex I/Q output signal for the rest of the processing chain.
// ---------------------------------------------------------------------
void CDelay::calc_bpf(double* pIn, complex* pOut)
{
complex acc;
const complex* Kptr;
complex* Firptr;
	for(int i = 0; i < BLOCKSIZE; i++) {
		pQue[FirState].re = pIn[i]; //place real values in circular Queue
		pQue[FirState].im = pIn[i];
		Firptr = pQue;
		Kptr = HilbertBPFirCoef + HILBPFIR_LENGTH - FirState;
		acc = complex(0, 0);
		for (int j = 0; j < HILBPFIR_LENGTH; j++) {
			acc += (*Firptr) * (*Kptr);
			Firptr++;
			Kptr++;
		}
		pOut[i] = acc;
		if (--FirState < 0)
			FirState = HILBPFIR_LENGTH - 1;
	}
}
