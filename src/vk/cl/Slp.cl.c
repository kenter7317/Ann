
#include "ae2f/Float.h"
#define ae2f_NEED_CLASS 0

#if defined(__ae2f_MACRO_GENERATED) && __ae2f_MACRO_GENERATED
#define ae2fVK_clspv_IS_OPENCL_C 1
#else
#define barrier(...)
#endif

#include <ae2fVK/clspv_clkeys.h>
#include <ae2f/Ann/Slp.h>

#ifndef ACT
#define ACT(r, x)
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(r, x)
#endif

#ifndef LOSS_DERIV
#define LOSS_DERIV(r, y, y_desired, i, c)
#endif

/** Magic numbers */
#define p_weight	glob
#define p_bias		(glob + (osz * isz))
#define p_inp		((p_bias) + (osz))
#define p_out		((p_inp) + isz)
#define p_delta		((p_out) + osz)
#define p_goal		((p_delta) + osz)

ae2f_structdef(struct, lr_t) {
	ae2f_float_t	m_weight;
	ae2f_float_t	m_bias;
};

/**
 * @brief
 *
 * get_global_id(0) : oidx	\n
 *
 * Global: \n
 * 	, ae2f_float_t[Out][Inp] : Weight	\n
 * 	, ae2f_float_t[Out] : Bias		\n
 * 	, ae2f_float_t[Inp]			\n
 * 	, ae2f_float_t[Out]			\n
 *
 * */
__kernel void kPredict(__global ae2f_float_t* glob, uint isz) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0);

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

	p_out[oidx] = (v_predict).m_ret;
}

/**
 * @brief
 *
 * get_global_id(0) : oidx	\n
 * get_local_id(0) : iidx	\n
 *
 * Global: \n
 * 	ae2f_float_t[Out][Inp] : Weights	\n
 * 	, ae2f_float_t[Out] : 	Bias		\n
 * 	, ae2f_float_t[Inp]			\n
 * 	, ae2f_float_t[Out]			\n
 * 	, ae2f_float_t[Out] : Delta		\n
 * 	, ae2f_float_t[Out] : Goal		\n
 *
 * Local: \n
 * 	ae2f_float_t[Out]		\n
 * */
__kernel void kTrain(lr_t lr, __global ae2f_float_t* glob, __local ae2f_float_t* loc) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1)
		;

#if 1
	ae2f_AnnSlpPredict_t	v_predict;
	ae2f_float_t		delta = 0, v_tmp = 0, v_tmp1 = 0;

	if(iidx == 0) {
		__ae2f_AnnSlpPredictOne_imp(
				v_predict
				, p_inp					/** prm_in */
				, p_weight				/** weight */
				, p_bias[oidx]				/** Bias */
				, ACT
				, oidx
				, isz
				);

		/** output */
		p_out[oidx] = loc[oidx] = (v_predict).m_ret;

		__ae2f_AnnSlpFetchDeltaOne_imp(
				v_tmp, v_tmp1
				, loc						/** out */
				, p_goal					/** out_desired */ 
				, ACT_DERIV, LOSS_DERIV
				, delta
				, oidx, osz
				);

		/** delta */
		p_delta[oidx] = (delta);

		__ae2f_AnnSlpFollowOneB_imp(
				p_bias[oidx] /** r_bias */
				, delta /** delta */
				, lr.m_bias
				);
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	__ae2f_AnnSlpFollowOneW_imp(
			p_inp[iidx] /** inp */
			, (delta) /** delta */
			, glob /** weight */
			, lr.m_weight
			, isz
			, iidx
			, osz
			, oidx
			);
#endif

}

/**
 * @brief
 *
 * get_global_id(0) : oidx	\n
 * get_local_id(0) : iidx	\n
 *
 * Global: \n
 * 	ae2f_float_t[Out][In] : Weights	\n
 * 	, ae2f_float_t[Out] : Bias	\n
 *	, ae2f_float_t[Inp]		\n
 *	, ae2f_float_t[Out]		\n
 * 	, ae2f_float_t[Out] : Delta	\n
 * 	, ae2f_float_t[Out] : Goal	\n
 *
 * */
__kernel void kFit(lr_t lr, __global ae2f_float_t* glob) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1)
		;

	ae2f_float_t	delta, v_tmp, v_tmp1;

	if(iidx == 0) {
		__ae2f_AnnSlpFetchDeltaOne_imp(
				v_tmp, v_tmp1
				, p_out /** out */
				, p_goal /** out_desired */ 
				, ACT_DERIV
				, LOSS_DERIV
				, delta /** retdelta */
				, oidx
				, osz
				);

		/** delta */
		p_delta[oidx] = delta;

		__ae2f_AnnSlpFollowOneB_imp(
				p_bias[oidx]	/** r_bias */
				, delta		/** delta */
				, lr.m_bias
				);
	}


	barrier(CLK_LOCAL_MEM_FENCE);

	__ae2f_AnnSlpFollowOneW_imp(
			(glob + osz * isz + osz)[iidx] /** inp */
			, delta /** delta */
			, p_weight /** weight */
			, lr.m_weight
			, isz
			, iidx
			, osz
			, oidx
			);
}

/**
 * @brief
 * get_global_id(0):	oidx
 * get_local_id(0):	iidx
 *
 * Global: \n
 * 	ae2f_float_t[Out][In] : Weights	\n
 * 	, ae2f_float_t[Out] : Bias	\n
 * 	, ae2f_float_t[Inp]		\n
 * 	, ae2f_float_t[Out] : Delta	\n
 * */
__kernel void kFollow(lr_t lr, __global ae2f_float_t* glob) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1)
		;

	__ae2f_AnnSlpFollowOneW_imp(
			p_weight[iidx] /** inp */
			, p_delta[oidx] /** delta */
			, p_weight /** weight */
			, lr.m_weight
			, isz
			, iidx
			, osz
			, oidx
			);

	if(iidx == 0) {
		__ae2f_AnnSlpFollowOneB_imp(
				p_bias[oidx] /** r_bias */
				, p_delta[oidx] /** delta */
				, lr.m_bias
				);
	}
}
