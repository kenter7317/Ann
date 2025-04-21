#ifndef Slp_cl_h
#define Slp_cl_h

#define _STDIO_H // Lowkey the stdio.h shit
#define ae2f_BInt_h

#include <ae2fCL/Loc.h>
#include <ae2f/Float.auto.h>
#include "uf.h"
#include "Sp.cl.h"

__kernel void ae2fCL_eAnnKernsSlpPredict(
	__global ae2f_float_t* in,
	ae2fCL_whenCL(__local) ae2f_float_t* loc
);

__kernel void ae2fCL_eAnnKernsSlpTrain( 
	__global ae2f_float_t* in
);

#endif
