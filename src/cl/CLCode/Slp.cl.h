#include <ae2fCL/Loc.h>
#include <ae2f/Float.h>
#include "uf.h"
#include "Sp.cl.h"


__kernel void ae2fCL_eAnnSlpPredict(
	__global void* in,
	ae2fCL_whenCL(__local) ae2f_float_t* loc
);

__kernel void ae2fCL_eAnnSlpTrain( 
	__global void* in 
#if ae2f_float_t_SIZE == cl_mem_SIZE 
	,ae2f_float_half_t LrErrA 
	,ae2f_float_half_t LrErrB 
#define LrErrTent() ae2f_float_t LrErr = \
	((uf_t) { LrErrA, LrErrB }).F
#else
	,ae2f_float_t LrErr 
#define LrErrTent()
#endif
);
