#define ae2f_NEED_CLASS 0

#if __ae2f_MACRO_GENERATED
#define ae2fVK_clspv_IS_OPENCL 1
#endif

#include "./Slp.auto.h"

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

const ae2f_structdef(struct, lr_t) {
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
__kernel void kPredict(__global ae2f_float_t* glob, const uint32_t unused) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1);

	clSlpPredict_t v_predict;

	clSlpPredict(v_predict, p_out, p_inp, p_weight, p_bias, iidx, isz, oidx, osz, ACT);
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

	ae2f_float_t		delta = 0, v_tmp = 0, v_tmp1 = 0;
	clSlpPredict_t		v_predict;

	clSlpPredict(v_predict, loc, p_inp, p_weight, p_bias, iidx, isz, oidx, osz, ACT);

	if(iidx == 0) {
		p_out[oidx] = loc[oidx];

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

	__ae2f_AnnSlpFollowOneW_imp(
			p_inp[iidx] /** inp */
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
			p_inp[iidx] /** inp */
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
