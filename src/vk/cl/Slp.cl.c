#pragma OPENCL EXTENSION cl-fast-relaxed-math : enable
#pragma OPENCL EXTENSION pod-pushconstant : enable

#if __ae2f_MACRO_GENERATED
#define ae2fVK_clspv_IS_OPENCL 1
#endif

#include "./Slp.auto.h"

#define ae2f_NEED_CLASS 0

#ifndef ACT
#define ACT(r, y, i, c)		*(r) = (y)[i];
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(r, y, i, c)		1
#endif

#ifndef LOSS_DERIV
#define LOSS_DERIV(r, y, y_desired, i, c) *(r) = (y)[i] - (y_desired)[i];
#endif

/** Magic numbers */
#define p_weight	glob
#define p_bias		(glob + (osz * isz))
#define p_inp		((p_bias) + (osz))
#define p_out		((p_inp) + (isz))
#define p_delta		((p_out) + osz)
#define p_goal		((p_delta) + osz)

#define loc		CAST(__local ae2f_float_t*, _loc)

const ae2f_structdef(struct, lr_t) {
	host_float_t	m_weight;
	host_float_t	m_bias;
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
__kernel void kPredict(__global host_float_t* restrict glob, const uint unused) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1);

	_clAtomAddF_t(__global, host_float_t)	v_predict;

	_clSlpPredict(
			__global
			, v_predict
			, p_out[oidx]
			, p_out
			, p_inp
			, p_weight
			, p_bias
			, iidx
			, isz
			, oidx
			, osz
			, ACT
			);
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
__kernel void kTrain(lr_t lr, __global host_float_t* restrict glob, __local uint* restrict _loc) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1)
		;

	ae2f_float_t		v_tmp = 0;
	_clAtomAddF_t(__local, ae2f_float_t)	slppredict;
#define delta	slppredict.m_atom[0].m_f
#define v_tmp1	slppredict.m_atom[1].m_f

	_clSlpPredict(__local, slppredict, v_tmp, loc, p_inp, p_weight, p_bias, iidx, isz, oidx, osz, ACT);

	if(iidx == 0) {
		p_out[oidx] = v_tmp;
		loc[oidx] = v_tmp;

		__ae2f_AnnSlpFetchDeltaOne_imp(
				delta
				, &v_tmp, &v_tmp1
				, loc						/** out */
				, p_goal					/** out_desired */ 
				, oidx, osz
				, ACT_DERIV, LOSS_DERIV
				);

		__ae2f_AnnSlpFollowOneB_imp(
				p_bias[oidx] /** r_bias */
				, delta /** delta */
				, lr.m_bias
				);

		p_delta[oidx] = delta;
	}

	delta = p_delta[oidx];
	v_tmp = p_inp[iidx];

	__ae2f_AnnSlpFollowOneW_imp(
			v_tmp
			, delta /** delta */
			, glob /** weight */
			, lr.m_weight
			, isz
			, iidx
			, osz
			, oidx
			);


#undef v_tmp1
#undef delta
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
__kernel void kFit(lr_t lr, __global host_float_t* restrict glob) {
	const size_t
		oidx = get_global_id(0)
		, osz = get_global_size(0)
		, iidx = get_global_id(1)
		, isz = get_global_size(1)
		;

	ae2f_float_t	delta, v_tmp, v_tmp1;

	if(iidx == 0) {
		__ae2f_AnnSlpFetchDeltaOne_imp(
				delta /** retdelta */
				, &v_tmp, &v_tmp1
				, p_out /** out */
				, p_goal /** out_desired */ 
				, oidx, osz
				, ACT_DERIV
				, LOSS_DERIV
				);

		/** delta */

		__ae2f_AnnSlpFollowOneB_imp(
				p_bias[oidx]	/** r_bias */
				, delta		/** delta */
				, lr.m_bias
				);

		p_delta[oidx] = delta;
	}

	delta = p_delta[oidx];
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
__kernel void kFollow(lr_t lr, __global host_float_t* restrict glob) {
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
