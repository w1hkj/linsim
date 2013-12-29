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

// pathsim code

#include <stdio.h>

#include "delay.h"
#include "filter_tables.h"

#define MAXDELAY (50*8)					// 50mSecs max delay
#define BLOCKSIZE 2048
#define BUFSIZE (BLOCKSIZE+MAXDELAY)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelay::CDelay()
{
	m_pDelayLine = new cmplx[BUFSIZE];
	m_pQue = new cmplx[HILBPFIR_LENGTH];
	m_Inptr = 0;
	m_T1ptr = 0;
	m_T2ptr = 0;
}

CDelay::~CDelay()
{
	if(m_pDelayLine)
		delete m_pDelayLine;
	if(m_pQue)
		delete m_pQue;
}

//////////////////////////////////////////////////////////////////////
//  Set delay times
//////////////////////////////////////////////////////////////////////
void CDelay::SetDelays( double t1, double t2)	//delays in msecs
{
	int i;
	for (i = 0; i < HILBPFIR_LENGTH; i++) {
		m_pQue[i] = cmplx(0,0);
	}
	for (i = 0; i < BUFSIZE; i++) {
		m_pDelayLine[i] = cmplx(0,0);
	}
	m_Inptr = BUFSIZE-1;
	m_FirState = HILBPFIR_LENGTH-1;

	int delay1 = (int)(8 * t1);
	int delay2 = (int)(8 * t2);

	if (delay1 > MAXDELAY) delay1 = MAXDELAY;
	m_T1ptr = BUFSIZE - delay1 - 1;
	if (m_T1ptr < 1) m_T1ptr = 1;

	if (delay2 > MAXDELAY) delay2 = MAXDELAY;
	m_T2ptr = BUFSIZE - delay2 - 1;
	if (m_T1ptr < 1) m_T1ptr = 1;
}

//////////////////////////////////////////////////////////////////////
//  Uses pointers to create variable delays
//////////////////////////////////////////////////////////////////////
void CDelay::CreateDelays( cmplx* inbuf, bool bt1, cmplx* t1buf, bool bt2, cmplx* t2buf )
{
	int i;
	if (!bt1 && !bt2) return;
	for (i = 0; i < BLOCKSIZE; i++) {
		m_pDelayLine[m_Inptr++] = inbuf[i]; // copy new data from inbuf into delay buffer
		if( m_Inptr >= BUFSIZE ) m_Inptr = 0;
		if (bt1) {
			t1buf[i] = m_pDelayLine[m_T1ptr++];
			if( m_T1ptr >= BUFSIZE ) m_T1ptr = 0;
		}
		if (bt2) {
			t2buf[i] = m_pDelayLine[m_T2ptr++];
			if (m_T2ptr >= BUFSIZE ) m_T2ptr = 0;
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Hilbert 3KHz BP filters.  Real input and complex I/Q output
//   This FIR bandwidth limits the real input as well as creates a
//   complex I/Q output signal for the rest of the processing chain.
//////////////////////////////////////////////////////////////////////
void CDelay::CalcBPFilter(double* pIn, cmplx* pOut)
{
	cmplx acc;
	const double* IKptr;
	const double* QKptr;
	cmplx* Firptr;
	int i, j;

	for (i = 0; i < BLOCKSIZE; i++) {
		m_pQue[m_FirState].x = pIn[i];	//place real values in circular Queue
		m_pQue[m_FirState].y = pIn[i];
		Firptr = m_pQue;
		IKptr = IHilbertBPFirCoef+HILBPFIR_LENGTH-m_FirState;
		QKptr = QHilbertBPFirCoef+HILBPFIR_LENGTH-m_FirState;
		acc.x = 0.0;
		acc.y = 0.0;
		for(j=0; j<	HILBPFIR_LENGTH;j++) {
			acc.x += ( (Firptr->x)*(*IKptr++) );
			acc.y += ( (Firptr++->y)*(*QKptr++) );
		}
		pOut[i].x = acc.x;
		pOut[i].y = acc.y;
		if( --m_FirState < 0) m_FirState = HILBPFIR_LENGTH-1;
	}
}

