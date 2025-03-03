/// @file Sp.cl.h

#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include "Size/cl_mem.auto.h"
#include "Size/ae2f_float_t.auto.h"


__kernel void ae2fCL_eAnnKernsSpPredict(
    __global ae2f_float_t* io,
    const __global ae2f_float_t* field,
    ae2fCL_whenCL(__local) ae2f_float_t* loc
);

/// @brief 
/// @param _in 
/// @param field 
/// @param LrErr 
/// @param in_idx 
/// @return 
__kernel void ae2fCL_eAnnKernsSpTrain(
    __global const ae2f_float_t* _in,
    __global ae2f_float_t* field,

    #if cl_mem_SIZE == ae2f_float_t_SIZE
        ae2f_float_half_t LrErrA,
        ae2f_float_half_t LrErrB
    #else
        ae2f_float_t LrErr
    #endif
);


#if ae2fCL_whenC(!) 0

#define get_local_id(...) 0
#define get_local_size(...) 0

#endif

#define ae2fCL_AnnDevSpPredict_Loc(in, field, loc, globalid) \
	loc[globalid] = in[globalid] * field[globalid]

#define _ae2fCL_AnnDevSpPredict_OutTent(localid, loci_v, blockSize, blocksz_v, halfBlockSize) \
	size_t \
	localid = loci_v, \
	blockSize = blocksz_v, \
	halfBlockSize = blockSize >> 1;

#define ae2fCL_AnnDevSpPredict_OutTent(localid, blockSize, halfBlockSize) \
	_ae2fCL_AnnDevSpPredict_OutTent( \
			localid, get_local_id(0), \
			blockSize, get_local_size(0), \
			halfBlockSize \
	)

#define ae2fCL_AnnDevSpPredict_Out(localid, blockSize, halfBlockSize, loc, out) \
	ae2fCL_whenCL(barrier(CLK_LOCAL_MEM_FENCE)); \
	while(halfBlockSize > 0) { \
		if (localid < halfBlockSize) { \
			loc[localid] += loc[localid + halfBlockSize]; \
	        	if ((halfBlockSize<<1)<blockSize) { \
                		if (localid==0) { \
                    			loc[localid] += loc[localid + (blockSize-1)]; \
                		} \
            		} \
        	} \
        	ae2fCL_whenCL(barrier(CLK_LOCAL_MEM_FENCE)); \
        	blockSize = halfBlockSize; \
        	halfBlockSize = blockSize >> 1; \
    	} \
	if (!localid) { \
        	out = loc[0]; \
    	}

#define ae2fCL_AnnDevSpTrain(_in, field, LrErr, i) ((field)[i] += (LrErr) * (_in)[i])
