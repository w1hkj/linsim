// ---------------------------------------------------------------------
// Path.h: interface for the CPath class.
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

#ifndef PATH_H
#define PATH_H

#include <math.h>
#include "filter_tables.h"
#include "gauss_fir.h"

class CPath  
{
public:
	void CalcPath( complex* pIn, complex* pOut);
	void InitPath(
		bool &active,
		double &Spread, double &Offset, int &blocksize,
		double &Samplerate, int &numpaths);
	CPath();
	virtual ~CPath();

private:
	complex		MakeGaussianDelaySample();

	int			m_IIRLength;
	int			m_NoiseSampRate;
	bool		m_PathActive;
	int			m_inc;
	int			m_BlockSize;
	int			m_Indx;
	double		m_Offset;
	double		m_Spread;
	double		m_LPGain;
	double	 	m_Timeinc;
	double		samplerate;

	complex		m_pQue0[INTP_QUE_SIZE];
	complex		m_pQue1[INTP_QUE_SIZE];
	complex		m_pQue2[INTP_QUE_SIZE];
	complex		m_pQue3[INTP_QUE_SIZE];
	int			m_FirState0;
	int			m_FirState1;
	int			m_FirState2;
	int			m_FirState3;

	CGaussFIR	*m_pLPFIR;

};

#endif
