// ---------------------------------------------------------------------
// NoiseGen.cpp: implementation of the CNoiseGen class.
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
//along with this program; if not, write to the
//
//  Free Software Foundation, Inc.
//  51 Franklin Street, Fifth Floor
//  Boston, MA  02110-1301 USA.
//
// ---------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "noise_gen.h"

// obtained experimentally to compensate for BP filter
// and give same results as PathSim for a 0 dB s/n TTY signal

#define K_ENBW 4.0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoiseGen::CNoiseGen()
{

}

CNoiseGen::~CNoiseGen()
{

}
void CNoiseGen::InitNoiseGen()
{
	for(int i=0; i<HILBPFIR_LENGTH; i++)
		m_pQue[i] = 0.0;		// fill delay buffer with zero
	m_FirState = HILBPFIR_LENGTH-1;
}

//////////////////////////////////////////////////////////////////
// Adds bufsize gaussian random doubles with 0 mean and
// RMSlevel = RMS = std to the specified buffer, pIn
//////////////////////////////////////////////////////////////////
void CNoiseGen::AddBWLimitedNoise(int bufsize, double *pIn, double siggain,double RMSlevel)
{
int i = 0, j = 0;
double rad;
double r;
double u1;
double u2;
const double* Kptr;
double* Firptr;

	RMSlevel = RMSlevel*K_ENBW;	//ENBW gain compensation(measured experimentally)
	while( i < bufsize ) {
// Generate two uniform random numbers between -1 and +1
// that are inside the unit circle
		do {
			u1 = 1.0 - 2.0 * (double)rand()/(double)RAND_MAX ;
			u2 = 1.0 - 2.0 * (double)rand()/(double)RAND_MAX ;
			r = u1*u1 + u2*u2;
		} while(r >= 1.0 || r == 0.0);
		rad = sqrt(-2.0*log(r)/r);

// Performance test stuff
//HistogramSamp(  u1*rad, -6.0, 6.0, 10000000 );
//HistogramSamp(  1.0 - 2.0 * (double)rand()/(double)RAND_MAX, -1.5, 1.5, 10000000 );
//pIn[i] = 0;
//pIn[i+1] = 0;

#if 1	//if want to use 3 Khz BP filtered noise set to 1 else is flat
//
// 3KHz BP filter the Gaussian noise(use one of the Hilbert 3Khz coefficient tables)
		m_pQue[m_FirState] = (RMSlevel*u1*rad);	//place in circular Queue
		Firptr = m_pQue;
		u1 = 0.0;
		Kptr = IHilbertBPFirCoef+HILBPFIR_LENGTH-m_FirState;
		for (j=0; j<	HILBPFIR_LENGTH;j++)
			u1 += ( (*Firptr++)*(*Kptr++) );
		if( --m_FirState < 0)
			m_FirState = HILBPFIR_LENGTH-1;
//  Add BP filtered noise to signal
		pIn[i] = siggain*pIn[i] + u1;
		i++;

		m_pQue[m_FirState] = (RMSlevel*u2*rad);	//place in circular Queue
		Firptr = m_pQue;
		u2 = 0.0;
		Kptr = IHilbertBPFirCoef+HILBPFIR_LENGTH-m_FirState;
		for (j=0; j<	HILBPFIR_LENGTH;j++)
			u2 += ( (*Firptr++)*(*Kptr++) );
		if( --m_FirState < 0)
			m_FirState = HILBPFIR_LENGTH-1;
//  Add BP filtered noise to signal
		pIn[i] = siggain*pIn[i] + u2;
		i++;
#else
		pIn[i] = siggain*pIn[i] + RMSlevel*u1*rad;
		i++;
		pIn[i] = siggain*pIn[i] + RMSlevel*u2*rad;
		i++;
#endif
	}
}

/*
#define K_ENBW 1.10		//obtained experimentally to compensate for BP filter
// ---------------------------------------------------------------------
// Construction/Destruction
// ---------------------------------------------------------------------

CNoiseGen::CNoiseGen()
{
}

CNoiseGen::~CNoiseGen()
{
}

void CNoiseGen::InitNoiseGen()
{
	for(int i = 0; i < HILBPFIR_LENGTH; i++)
		m_pQue[i] = 0.0;		// fill delay buffer with zero
	m_FirState = HILBPFIR_LENGTH-1;
}

// ---------------------------------------------------------------------
// Adds bufsize gaussian random doubles with 0 mean and
// RMSlevel = RMS = std to the specified buffer, pIn
// ---------------------------------------------------------------------
void CNoiseGen::AddBWLimitedNoise(int bufsize, double *pIn, double siggain, double RMSlevel)
{
int i = 0;
double rad;
double r;
double u1;
double u2;
const complex *Kptr;
double* Firptr;

	RMSlevel = RMSlevel*K_ENBW;	//ENBW gain compensation(measured experimentally)
	while (i < bufsize) {
// Generate two uniform random numbers between -1 and +1
// that are inside the unit circle
		do {
			u1 = 1.0 - 2.0 * (double)rand()/(double)RAND_MAX ;
			u2 = 1.0 - 2.0 * (double)rand()/(double)RAND_MAX ;
			r = u1*u1 + u2*u2;
		} while (r >= 1.0 || r == 0.0);
		rad = sqrt(-2.0*log(r)/r);

#if 1	//if want to use 3 Khz BP filtered noise set to 1 else is flat
//
// 3KHz BP filter the Gaussian noise
// use real component of the Hilbert 3Khz coefficient tables
		m_pQue[m_FirState] = (RMSlevel*u1*rad);	//place in circular Queue
		Firptr = m_pQue;
		u1 = 0.0;
		Kptr = HilbertBPFirCoef + HILBPFIR_LENGTH - m_FirState;
		for (int j = 0; j < HILBPFIR_LENGTH; j++) {
			u1 += *Firptr * Kptr->re;
			Firptr++;
			Kptr++;
		}
		if( --m_FirState < 0)
			m_FirState = HILBPFIR_LENGTH - 1;
//  Add BP filtered noise to signal
		pIn[i] = siggain*pIn[i++] + u1;

		m_pQue[m_FirState] = (RMSlevel*u2*rad);	//place in circular Queue
		Firptr = m_pQue;
		u2 = 0.0;
		Kptr = HilbertBPFirCoef + HILBPFIR_LENGTH - m_FirState;
		for (int j = 0; j < HILBPFIR_LENGTH; j++) {
			u2 += *Firptr * Kptr->re;
			Firptr++;
			Kptr++;
		}
		if( --m_FirState < 0)
			m_FirState = HILBPFIR_LENGTH - 1;
//  Add BP filtered noise to signal
		pIn[i] = siggain*pIn[i++] + u2;
#else
		pIn[i] = siggain*pIn[i++] + RMSlevel*u1*rad;
		pIn[i] = siggain*pIn[i++] + RMSlevel*u2*rad;
#endif
	}

}
*/
