// ---------------------------------------------------------------------
// GaussFIR.cpp: implementation of the CGaussFIR class.
//
//    A Gaussian shaped cmplx FIR LP filter class.
// ---------------------------------------------------------------------
// Copyright 2000, Moe Wheatley AE4JY <ae4jy@mindspring.com>
// Modified  2013, Dave Freese W1HKJ  <w1hkj@w1hkj.com>
//
//  based on the program PathSim by Moe Wheatley AE4JY
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
#include <math.h>
#include "gauss_fir.h"

// pathsim original

#define PI2 6.283185307
#define SQRT2PI 2.506628275
#define SQRT2 1.414213562
#define K_GAUSSIAN 1.4	//constant determines accuracy of Gaussian LPFIR.
						// larger number makes FIR longer but more accurate
						// This value is good to about -50 dB

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGaussFIR::CGaussFIR()
{
	m_pCoef = NULL;
	m_pQue = NULL;
}

CGaussFIR::~CGaussFIR()
{
	if(m_pCoef)
		delete m_pCoef;
	if(m_pQue)
		delete m_pQue;
}

////////////////////////////////////////////////////////////////////////////
//  This function calculates the length and FIR coefficients for a Gaussian
//		shaped LP filter and also initializes the memory for it.
////////////////////////////////////////////////////////////////////////////
void CGaussFIR::Init(double Fs, double F2sig)
{
int indx;
double sigma = (Fs*SQRT2)/(PI2*F2sig);		// calculate sigma for coefficients
	m_FIRlen = (int)(K_GAUSSIAN*Fs/F2sig);		//calculate FIR length
	if( !(m_FIRlen & 1) ) //make FIR length ODD
		m_FIRlen++;
//allocate buffer and Coefficient memory based on calculated FIR length
	if( (m_pCoef = new double[sizeof(double)*((m_FIRlen*2)+10)] ) == NULL)
		return;
	if( (m_pQue = new cmplx[sizeof(cmplx)*(m_FIRlen+10)] ) == NULL)
		return;
// generate the scaled Gaussian shaped impulse response	to create a 0 dB
//   passband LP filter with a 2 Sigma frequency bandwidth.
	indx = -(m_FIRlen-1)/2;
	for(int i=0; i<m_FIRlen;i++)
	{
		m_pCoef[i] = ( 1.0/(SQRT2PI*sigma) )*dnorm( indx++,0.0, sigma)
									/ dnorm( 0.0,0.0, sigma);
		m_pQue[i].x = 0.0;
		m_pQue[i].y = 0.0;
		m_pCoef[i+m_FIRlen] = m_pCoef[i];	//make duplicate for flat FIR
	}
	m_FirState = m_FIRlen-1;		//used for flat FIR implementation
}

//////////////////////////////////////////////////////////////////////
//  Calculate complex Gaussian FIR filter iteration for one sample
//////////////////////////////////////////////////////////////////////
cmplx CGaussFIR::CalcFilter(cmplx in)
{
cmplx acc;
cmplx* Firptr;
double* Kptr;
	m_pQue[m_FirState].x = in.x;
	m_pQue[m_FirState].y = in.y;
	Firptr = m_pQue;
	acc.x = 0.0;
	acc.y = 0.0;
	Kptr = m_pCoef+m_FIRlen-m_FirState;
	for(int i=0; i<m_FIRlen; i++)
	{
		acc.x += ( (Firptr->x)*(*Kptr) );
		acc.y += ( (Firptr++->y)*(*Kptr++) );
	}
	if( --m_FirState < 0)
		m_FirState += m_FIRlen;
	return acc;
}

//////////////////////////////////////////////////////////////////////
//  implements a Gaussian (Normal) distribution function.
//////////////////////////////////////////////////////////////////////
double CGaussFIR::dnorm(double x, double mu, double sigma)
{
	return( 1.0/(SQRT2PI*sigma) )*exp( (-1.0/(2.0*sigma*sigma))*(x-mu)*(x-mu) );
}
