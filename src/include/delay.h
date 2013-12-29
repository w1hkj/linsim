// ---------------------------------------------------------------------
// Delay.h: interface for the CDelay class.
//
// ---------------------------------------------------------------------
// Copyright 2000, Moe Wheatley AE4JY <ae4jy@mindspring.com>
// Modified  2013, Dave Freese W1HKJ  <w1hkj@w1hkj.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 
// 59 Temple Place - Suite 330
// Boston, MA  02111-1307, USA.
// ---------------------------------------------------------------------

#ifndef DELAY_H
#define DELAY_H

#include <math.h>
#include "cmplx.h"

class CDelay  
{
public:
	void CreateDelays( cmplx* inbuf, bool bt1, cmplx* t1buf, bool bt2, cmplx* t2buf );
	void SetDelays( double t1, double t2);
	void CalcBPFilter(double* pIn, cmplx* pOut);
	CDelay();
	virtual ~CDelay();

private:
	int m_FirState;
	cmplx* m_pDelayLine;
	cmplx* m_pQue;
	int m_T1ptr;
	int m_T2ptr;
	int m_Inptr;
};

#endif
