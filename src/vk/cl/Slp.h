#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include <ae2f/Ann/Slp.h>
#include <ae2fVK/clspv_clkeys.h>
#include "mac.h"

#if !__ae2f_MACRO_GENERATED
#endif

typedef ae2f_AnnSlpPredict_t _clSlpPredict_t;

ae2f_MAC() clSlpPredict(
		_clSlpPredict_t	v_predict,
		ae2f_float_t*	const	  loc,

		const ae2f_float_t* const p_inp,
		const ae2f_float_t* const p_weight,
		const ae2f_float_t* const p_bias,

		const size_t iidx, const size_t isz, 
		const size_t oidx, const size_t osz,

		ae2f_AnnAct_t	ACT
		)
{
	if((iidx) == 0 && (oidx) < (osz)) {
		__ae2f_AnnSlpPredictOne_imp(
				(v_predict)
				, (p_inp)					/** prm_in */
				, (p_weight)				/** weight */
				, (p_bias)[oidx]				/** Bias */
				, ACT
				, (oidx)
				, (isz)
				);

		(loc)[oidx] = (v_predict).m_ret;
	}

	barrier(CLK_ALL_MEM_FENCE);
}

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
		(v_predict).m_sum = work_group_reduce_add(
				(p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx]
				);

		if((iidx) == 0) {
			ACT((&(v_predict).m_ret), ((v_predict).m_sum + (p_bias)[oidx]));
			(loc)[oidx] = (v_predict).m_ret;
		}
	}
}

#define clSlpPredict	CL_Q_CVRT(_clSlpPredict)
#define clSlpPredict_t	CL_Q_CVRT(_clSlpPredict_t)

#endif
