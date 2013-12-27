// ---------------------------------------------------------------------
// GaussFIR.h: interface for the CGaussFIR class.
//
//    A Gaussian shaped complex FIR LP filter class.
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

#ifndef GUASS_H
#define GAUSS_H

#include <math.h>
#include "complex.h"

class CGaussFIR  
{
public:
	void Init( double Fs, double F2sig );
	CGaussFIR();
	virtual ~CGaussFIR();
	complex CalcFilter(complex in);

private:
	double* m_pCoef;
	int m_FIRlen;
	int m_FirState;
	complex* m_pQue;

	double dnorm(double x, double mu, double sigma);
};

#endif // !defined(AFX_GAUSSFIR_H__D7BB1864_A285_11D4_B4E9_705850C10000__INCLUDED_)
