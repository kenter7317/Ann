#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include <ae2f/Ann/Slp.h>
#include <ae2fVK/clspv_clkeys.h>

#if !__ae2f_MACRO_GENERATED
#endif

ae2f_MAC() clSlpPredict(
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
		ae2f_AnnSlpPredict_t	v_predict;
		__ae2f_AnnSlpPredictOne_imp(
				v_predict
				, p_inp					/** prm_in */
				, p_weight				/** weight */
				, p_bias[oidx]				/** Bias */
				, ACT
				, oidx
				, isz
				);

		(loc)[oidx] = (v_predict).m_ret;
	}
}

/** Quick version, not precise. */
ae2f_MAC() clSlpPredict_Q(
		__local ae2f_float_t*	const	  loc,

		const ae2f_float_t* const p_inp,
		const ae2f_float_t* const p_weight,
		const ae2f_float_t* const p_bias,

		const size_t iidx, const size_t isz, 
		const size_t oidx, const size_t osz,

		ae2f_AnnAct_t	ACT
		)
{
	if((oidx) < (osz)) {
		const ae2f_float_t v_sum = work_group_reduce_add(
				(p_weight)[(oidx) * (isz) + (iidx) * (p_inp)[iidx]]
				);
		barrier(CLK_ALL_MEM_FENCE);

		if((iidx) == 0) {
			ACT(((loc) + (oidx)), ((v_sum) + (p_bias)[oidx]));
		}
	}
}

#endif
