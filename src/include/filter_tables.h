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

#ifndef FilterTablesXYZ
#define FilterTablesXYZ

#include <math.h>

#define PI2 (M_PI * 2.0)
#define HILBPFIR_LENGTH 59

extern double IHilbertBPFirCoef[];
extern double QHilbertBPFirCoef[];
extern void init_BPFir(double sr);

///////////////////////////////////////////////////
//  Filter type:   FIR filter (5X Interpolation LP)
//  Design method: Parks-McClellan method
//  Number of taps  =  50 
//  Number of bands =  2 
//  Band     Lower       Upper       GN Value    Weight
//            edge        edge
//
//   1			0.0			.03125		5.		  1.
//   2			.1			.5			.00001    10.
////////////////////////////////////////////////////
#define INTP_FIR_SIZE 50
#define INTP_VALUE 5
#define INTP_QUE_SIZE (INTP_FIR_SIZE/INTP_VALUE)

const double X5IntrpFIRCoef[INTP_FIR_SIZE*2] = {
   0.0017373164,
   0.0036097115,
   0.0065540159,
    0.010062068,
    0.013405582,
    0.015465195,
    0.014866029,
    0.010251076,
  0.00066805735,
   -0.014014494,
   -0.032719002,
   -0.052952292,
   -0.070832208,
   -0.081436194,
   -0.079445844,
   -0.060037751,
   -0.019846439,
    0.042136111,
     0.12396486,
     0.22055661,
     0.32405698,
     0.42468841,
     0.51203738,
     0.57649606,
     0.61072876,
     0.61072876,
     0.57649606,
     0.51203738,
     0.42468841,
     0.32405698,
     0.22055661,
     0.12396486,
    0.042136111,
   -0.019846439,
   -0.060037751,
   -0.079445844,
   -0.081436194,
   -0.070832208,
   -0.052952292,
   -0.032719002,
   -0.014014494,
  0.00066805735,
    0.010251076,
    0.014866029,
    0.015465195,
    0.013405582,
    0.010062068,
   0.0065540159,
   0.0036097115,
   0.0017373164,
//
   0.0017373164,
   0.0036097115,
   0.0065540159,
    0.010062068,
    0.013405582,
    0.015465195,
    0.014866029,
    0.010251076,
  0.00066805735,
   -0.014014494,
   -0.032719002,
   -0.052952292,
   -0.070832208,
   -0.081436194,
   -0.079445844,
   -0.060037751,
   -0.019846439,
    0.042136111,
     0.12396486,
     0.22055661,
     0.32405698,
     0.42468841,
     0.51203738,
     0.57649606,
     0.61072876,
     0.61072876,
     0.57649606,
     0.51203738,
     0.42468841,
     0.32405698,
     0.22055661,
     0.12396486,
    0.042136111,
   -0.019846439,
   -0.060037751,
   -0.079445844,
   -0.081436194,
   -0.070832208,
   -0.052952292,
   -0.032719002,
   -0.014014494,
  0.00066805735,
    0.010251076,
    0.014866029,
    0.015465195,
    0.013405582,
    0.010062068,
   0.0065540159,
   0.0036097115,
   0.0017373164
 };

#endif
