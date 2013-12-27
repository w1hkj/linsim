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

#ifndef FilterTablesXYZ
#define FilterTablesXYZ

#include <math.h>
#include "complex.h"

#define HILBPFIR_LENGTH 59

// ---------------------------------------------------------------------
//#define PI2 ( 2.0 * M_PI )		// 2 Pi

// ---------------------------------------------------------------------
//  Gain compensation for the LP filters.
// gain = sqrt( (Fs/2)/(1.047*LPBW) )

// Filter type:   FIR filter
//  Design method: Parks-McClellan method
//  Number of taps  =  59 
//  Number of bands =  2 
//  Band     Lower       Upper       Value      Weight
//           edge        edge
//
//   1    0.0			.1875			1.		1.
//   2    .2375			.5			.00001		1.
// This created a 1.5KHz LP FIR which was then converted into a
//  3KHz Hilbert BP filter with the following command line program:
//
// ---------------------------------------------------------------------
//                     HILBERT Xform   P R O G R A M                  //
// ---------------------------------------------------------------------

//int main( int argc, char *argv[] )
//{
//double hlp, ibp, qbp, cfreq, Fs, Wo;
//int linecount, N;
//int ix;
//	N = argc;
//   ix = 0;
// get input file name
//   while( argv[1][ix] != NULL && argv[1][ix] != '.' ){
//      outfile1[ix] = argv[1][ix];
//      outfile2[ix] = argv[1][ix];
//      ix++;
//   }
//   outfile1[ix] = '.';  outfile2[ix++] = '.';
//   outfile1[ix] = 'i';  outfile2[ix++] = 'q';
//   outfile1[ix] = 'b';  outfile2[ix++] = 'b';
//   outfile1[ix] = 'p';  outfile2[ix++] = 'p';
//   outfile1[ix] = NULL; outfile2[ix++] = NULL;
//
//   printf( "\nEnter Sample frequency (Hz)-");
//  	ix = scanf("%lf", &Fs );
//   printf("\n Sample Freq = %8.4f",Fs );
//   printf( "\nEnter Bandpass Center Frequency (Hz)-");
//  	ix = scanf("%lf", &cfreq );
//   printf("\n Center Freq = %8.4f",cfreq );
//   Wo = 2 * 3.14159 * cfreq;
/*--------------------------------------------------------------------------*/
/*   See if can open specified file for reading & writing                   */
/*--------------------------------------------------------------------------*/
//
//   if( (strmin = fopen(argv[1],"r")) != NULL && 
//      (strmout1 = fopen(outfile1,"w")) != NULL &&
//      (strmout2 = fopen(outfile2,"w")) != NULL )   {
//         printf("\nFile [ %s ] is being processed.\n", argv[1]);
//      N = 0;
// find filter length
//      do{
//         newstr = fgets(string,MAXTAPS-1,strmin);
//         if( newstr != NULL ){
//            N++;
//         }
//      }while( newstr != NULL );
//      printf("\n Filter Length = %d\n\n",N );
//
//      rewind( strmin );
/*--------------------------------------------------------------------------*/
/*   Read file into buffer one line at a time, till the                 	*/
/*   the EOF is reached.                                                	*/
//	  fprintf(strmout1,"const double HilBPI[] {\n" );
//	  fprintf(strmout2,"const double HilBPQ[] {\n" );
//      linecount = 0;
//      do{
//         newstr = fgets(string,MAXTAPS-1,strmin);
//         if( newstr != NULL ){
//            hlp = atof( newstr );     // convert coeficient string  to float
// apply transform to low pass filter coefficients
//            ibp = (.707106781)*2.0 * hlp * cos( (Wo/Fs)*(linecount-( (N-1)/2 ) ) );
//            qbp = (.707106781)*2.0 * hlp * sin( (Wo/Fs)*(linecount-( (N-1)/2 ) ) );
//
//            fprintf(strmout1,"\t%10.8g,\n",ibp );
//            fprintf(strmout2,"\t%10.8g,\n",qbp );
//            linecount++;
//         }
//      }while( newstr != NULL );
//	  fprintf(strmout1,"\n};" );
//	  fprintf(strmout2,"\n};" );
//      printf("\nConverted %d coefficients.\n", linecount);
//      printf("\nOutput files are [ %s ], [ %s ]", outfile1, outfile2 );
//		fclose(strmin);
//		fclose(strmout1);
//		fclose(strmout2);
//   }
/*--------------------------------------------------------------------------*/
/* Here if can't open the specified input/output files 						*/
/*--------------------------------------------------------------------------*/
//   else {
//      printf("\n\nSpecified file(s) cannot be opened.\n");
//   }
//   return(0);
//}
/*+++++++++++++++++++++   E N D   P R O G R A M   ++++++++++++++++++++++++++*/

const complex HilbertBPFirCoef [HILBPFIR_LENGTH * 2] = {
	complex( 0.0015461946, 0.0013205749),
	complex( 0.00096844503, -0.0013329502),
	complex( 0.003185104, 0.0019518342),
	complex( -0.00045313707, 0.00088933157),
	complex( 0.0040178359, 0.0016642421),
	complex( -0.0010694961, 0.0032915705),
	complex( 0.0046038332, 0.0011052826),
	complex( -0.0012170725, 0.0076842931),
	complex( 0.0022920463, 0.00018038796),
	complex( -3.5363822e-017, 0.012029604),
	complex( -0.0033438091, 0.00026316348),
	complex( 0.0022776352, 0.014380423),
	complex( -0.012046394, 0.0028920833),
	complex( 0.0039531812, 0.012166641),
	complex( -0.021553635, 0.0089278078),
	complex( 0.00201261, 0.0039499695),
	complex( -0.028298385, 0.017341273),
	complex( -0.0070138471, -0.0096537323),
	complex( -0.02851387, 0.024353146),
	complex( -0.025490301, -0.025490301),
	complex( -0.019959804, 0.023369928),
	complex( -0.053128174, -0.038599878),
	complex( -0.0032769397, 0.0053474796),
	complex( -0.086207816, -0.043925076),
	complex( 0.017691961, -0.042712172),
	complex( -0.11813811, -0.038385399),
	complex( 0.036951781, -0.15391524),
	complex( -0.14135815, -0.022388932),
	complex( 0.048464111, -0.61579492),
	complex( 0.85015276,   0.0),
	complex( 0.048464111, 0.61579492),
	complex( -0.14135815, 0.022388932),
	complex( 0.036951781, 0.15391524),
	complex( -0.11813811, 0.038385399),
	complex( 0.017691961, 0.042712172),
	complex( -0.086207816, 0.043925076),
	complex( -0.0032769397, -0.0053474796),
	complex( -0.053128174, 0.038599878),
	complex( -0.019959804, -0.023369928),
	complex( -0.025490301, 0.025490301),
	complex( -0.02851387, -0.024353146),
	complex( -0.0070138471, 0.0096537323),
	complex( -0.028298385, -0.017341273),
	complex( 0.00201261, -0.0039499695),
	complex( -0.021553635, -0.0089278078),
	complex( 0.0039531812, -0.012166641),
	complex( -0.012046394, -0.0028920833),
	complex( 0.0022776352, -0.014380423),
	complex( -0.0033438091, -0.00026316348),
	complex( -3.5363822e-017, -0.012029604),
	complex( 0.0022920463, -0.00018038796),
	complex( -0.0012170725, -0.0076842931),
	complex( 0.0046038332, -0.0011052826),
	complex( -0.0010694961, -0.0032915705),
	complex( 0.0040178359, -0.0016642421),
	complex( -0.00045313707, -0.00088933157),
	complex( 0.003185104, -0.0019518342),
	complex( 0.00096844503, 0.0013329502),
	complex( 0.0015461946, -0.0013205749 ),
//Duplicate coefficients again for flat wrap around FIR
	complex( 0.0015461946, 0.0013205749),
	complex( 0.00096844503, -0.0013329502),
	complex( 0.003185104, 0.0019518342),
	complex( -0.00045313707, 0.00088933157),
	complex( 0.0040178359, 0.0016642421),
	complex( -0.0010694961, 0.0032915705),
	complex( 0.0046038332, 0.0011052826),
	complex( -0.0012170725, 0.0076842931),
	complex( 0.0022920463, 0.00018038796),
	complex( -3.5363822e-017, 0.012029604),
	complex( -0.0033438091, 0.00026316348),
	complex( 0.0022776352, 0.014380423),
	complex( -0.012046394, 0.0028920833),
	complex( 0.0039531812, 0.012166641),
	complex( -0.021553635, 0.0089278078),
	complex( 0.00201261, 0.0039499695),
	complex( -0.028298385, 0.017341273),
	complex( -0.0070138471, -0.0096537323),
	complex( -0.02851387, 0.024353146),
	complex( -0.025490301, -0.025490301),
	complex( -0.019959804, 0.023369928),
	complex( -0.053128174, -0.038599878),
	complex( -0.0032769397, 0.0053474796),
	complex( -0.086207816, -0.043925076),
	complex( 0.017691961, -0.042712172),
	complex( -0.11813811, -0.038385399),
	complex( 0.036951781, -0.15391524),
	complex( -0.14135815, -0.022388932),
	complex( 0.048464111, -0.61579492),
	complex( 0.85015276,   0.0),
	complex( 0.048464111, 0.61579492),
	complex( -0.14135815, 0.022388932),
	complex( 0.036951781, 0.15391524),
	complex( -0.11813811, 0.038385399),
	complex( 0.017691961, 0.042712172),
	complex( -0.086207816, 0.043925076),
	complex( -0.0032769397, -0.0053474796),
	complex( -0.053128174, 0.038599878),
	complex( -0.019959804, -0.023369928),
	complex( -0.025490301, 0.025490301),
	complex( -0.02851387, -0.024353146),
	complex( -0.0070138471, 0.0096537323),
	complex( -0.028298385, -0.017341273),
	complex( 0.00201261, -0.0039499695),
	complex( -0.021553635, -0.0089278078),
	complex( 0.0039531812, -0.012166641),
	complex( -0.012046394, -0.0028920833),
	complex( 0.0022776352, -0.014380423),
	complex( -0.0033438091, -0.00026316348),
	complex( -3.5363822e-017, -0.012029604),
	complex( 0.0022920463, -0.00018038796),
	complex( -0.0012170725, -0.0076842931),
	complex( 0.0046038332, -0.0011052826),
	complex( -0.0010694961, -0.0032915705),
	complex( 0.0040178359, -0.0016642421),
	complex( -0.00045313707, -0.00088933157),
	complex( 0.003185104, -0.0019518342),
	complex( 0.00096844503, 0.0013329502),
	complex( 0.0015461946, -0.0013205749 )
};

// ---------------------------------------------------------------------
//  Filter type:   FIR filter (5X Interpolation LP)
//  Design method: Parks-McClellan method
//  Number of taps  =  50 
//  Number of bands =  2 
//  Band     Lower       Upper       GN Value    Weight
//            edge        edge
//
//   1			0.0			.03125		5.		  1.
//   2			.1			.5			.00001    10.
// ---------------------------------------------------------------------
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
