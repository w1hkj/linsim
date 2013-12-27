//----------------------------------------------------------------------
// fft.h: interface for the Cfft class.
//
//  This is a slightly modified version of Takuya OOURA's
//     original radix 4 FFT package.
//Copyright(C) 1996-1998 Takuya OOURA
//    (email: ooura@mmm.t.u-tokyo.ac.jp).
//----------------------------------------------------------------------
// modified for use on Linux
// Modified 2006-2013. Dave Freese W1HKJ <w1hkj@w1hkj.com>
//----------------------------------------------------------------------

#ifndef CFFT_H
#define CFFT_H

#define FFT_SIZE 2048

class Cfft  
{
public:
	Cfft();
	~Cfft();

	void CalcFFT(double * InBuf);
	void ResetFFT();
	void SetFFTParams(  int ave, double gain, int type);
	bool GetFFTData( int start, int stop, long* OutBuf);

private:
	bool Overload;
	int LastAve;
	double Gain;
	double Clip;
	bool LogMode;
	bool LastLogMode;
	int AveSize;
	double * SinCosTbl;
	double * WindowTbl;
	double* pFFTAveBuf;
	double* pFFTInBuf;
	int * WorkArea;
	void makewt(int nw, int *ip, double *w);
	void makect(int nc, int *ip, double *c);
	void bitrv2(int n, int *ip, double *a);
	void cftfsub(int n, double *a, double *w);
	void rftfsub(int n, double *a, int nc, double *c);
    void cft1st(int n, double *a, double *w);
    void cftmdl(int n, int l, double *a, double *w);
};

#endif
