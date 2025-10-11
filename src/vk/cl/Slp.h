#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include "mac.h"
#include <ae2f/Ann/Slp.core.h>
#include <ae2f/Ann/Slp.auto.h>


#if !__ae2f_MACRO_GENERATED
#define _clAtomAddF_tmpl(a, b, c, d, e)	clAtomAddF_tmpl(d, e)
#define host_float_uint_t	uint
#define _clAtomAddF(a, b, c)		clAtomAddF(b, c)
#endif

ae2f_MAC(__global, host_float_t, host_float_uint_t, ) clAtomAddF_tmpl(
		__global volatile host_float_t* const prm_dst, 
		const ae2f_float_t prm_val
		)
{
	union { host_float_t m_f; host_float_uint_t m_u; } v_mem[2];
	(v_mem)[0].m_f = *(prm_dst);
	(v_mem)[1].m_f = (v_mem)[0].m_f + (host_float_t)(prm_val);
	atom_cmpxchg(
			(__global volatile host_float_uint_t* const)prm_dst
			, (v_mem)[0].m_u
			, (v_mem)[1].m_u
		    );
}

ae2f_MAC(__global, ) clAtomAddF(__global volatile host_float_t* prm_dst, ae2f_float_t prm_val)
{
	switch(sizeof(host_float_t)) {
		case sizeof(uint16_t):
			_clAtomAddF_tmpl(__global, host_float_t, uint16_t, prm_dst, prm_val);
			break;
		case sizeof(uint32_t):
			_clAtomAddF_tmpl(__global, host_float_t, uint32_t, prm_dst, prm_val);
			break;
		case sizeof(uint64_t):
			_clAtomAddF_tmpl(__global, host_float_t, uint64_t, prm_dst, prm_val);
			break;
	}
}

#if 1

ae2f_MAC(__global, ) clSlpPredict(
		ae2f_float_t	ret,
		__local ae2f_float_t* const loc,
		const __global ae2f_float_t* const p_inp,
		const __global ae2f_float_t* const p_weight,
		const __global ae2f_float_t* const p_bias,
		const size_t iidx, const size_t isz, 
		const size_t oidx, const size_t osz,
		ae2f_AnnAct_t ACT
		)
{
	if((oidx) < (osz) && (iidx) < (isz)) {
		unless((iidx)) (loc)[oidx] = 0;
		ae2f_float_t v_tmp = (p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx];
		_clAtomAddF(__global, &(loc)[oidx], v_tmp);
		unless(iidx) {
			(loc)[oidx] += (p_bias)[oidx];
			ACT(&(ret), (loc), oidx, osz);
		}
	}

}

#endif

#define clSlpPredict	CL_Q_CVRT(_clSlpPredict)
#define clSlpPredict_t	CL_Q_CVRT(_clSlpPredict_t)

#endif
