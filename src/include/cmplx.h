#ifndef __COMPLEX_H
#define __COMPLEX_H

struct cmplx { 
	double x; 
	double y;
	cmplx() {x = 0; y = 0; }
	cmplx(double a, double b) { x = a; y = b;}
};

#endif
