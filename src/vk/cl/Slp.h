#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include <ae2fVK/clspv_clkeys.h>
#include <ae2f/Ann/Slp.core.h>
#include <ae2f/Ann/Slp.auto.h>
#include "mac.h"

typedef ae2f_AnnSlpPredictOne_t _clSlpPredict_t;

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
		_clSlpPredict_t	_v_predict;
		(_v_predict).m_tmp = 0;
		for((_v_predict).m_j = (isz); (_v_predict).m_j--;) {
			(_v_predict).m_tmp 
				+= p_inp[(_v_predict).m_j] * p_weight[(_v_predict).m_j + (isz) * (oidx)];
		}

		(_v_predict).m_tmp += (p_bias)[oidx];
		ACT(&(_v_predict).m_ret, (_v_predict).m_tmp);
		(loc)[oidx] = (_v_predict).m_ret;
	}
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
