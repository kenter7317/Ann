#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include <ae2fVK/clspv_clkeys.h>
#include <ae2f/Ann/Slp.core.h>
#include <ae2f/Ann/Slp.auto.h>
#include "mac.h"


#if 1

ae2f_structdef(struct, _clSlpPredict_t) {
	ae2f_float_t	m_sum, m_ret;
};

ae2f_MAC() clSlpPredict(
		_clSlpPredict_t	v_predict,
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
		if(!(iidx)) (loc)[oidx] = 0;
		barrier(CLK_ALL_MEM_FENCE);

		(void)atomic_add(
				&(loc)[oidx]
				, (p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx]
				);

		if(!(iidx)) {
			(loc)[oidx] += (p_bias)[oidx];
			ACT((&(v_predict).m_ret), (loc), oidx, osz);
			barrier(CLK_ALL_MEM_FENCE);
			(loc)[oidx] = (v_predict).m_ret;
		}
	}

	barrier(CLK_ALL_MEM_FENCE);
}

#endif

ae2f_structdef(struct, _clSlpPredict_t_Q) {
	ae2f_float_t	m_sum, m_ret;
};

/** Quick version, not precise. */
ae2f_MAC() clSlpPredict_Q(
		_clSlpPredict_t_Q	v_predict,

		__local ae2f_float_t*	const	  loc,

		const ae2f_float_t* const p_inp,
		const ae2f_float_t* const p_weight,
		const ae2f_float_t* const p_bias,

		const size_t iidx, const size_t isz, 
		const size_t oidx, const size_t osz,

		ae2f_AnnAct_t	ACT
		)
{
	if((oidx) < (osz) && (iidx) < (isz)) {
		(loc)[oidx] = work_group_reduce_add(
				(p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx]
				) + (p_bias)[oidx];

		if(!(iidx)) {
			ACT((&(v_predict).m_ret), loc, oidx, osz);
			barrier(CLK_ALL_MEM_FENCE);
			(loc)[oidx] = (v_predict).m_ret;
		}
	}
}

#define clSlpPredict	CL_Q_CVRT(_clSlpPredict)
#define clSlpPredict_t	CL_Q_CVRT(_clSlpPredict_t)

#endif
