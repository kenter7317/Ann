
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
#define ACT(x)		(x)
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(x)	(x)
#endif

#ifndef LOSS_DERIV
#define LOSS_DERIV(y, y_desired, i, c) 0
#endif

#define __CC(T)		__constant const T
#define __CC_P(T)	__constant const T*

#define __LO(T)		__local T
#define __LO_P(T)	__local T*

#define __CAST(Q, T, v)	ae2f_reinterpret_cast(Q(T), v)

#define cast_LO(T, v)	__CAST(__LO, T, v)
#define cast_LO_P(T, v)	__CAST(__LO_P, T, v)

#define cast_CC(T, v)	__CAST(__CC, T, v
#define cast_CC_P(T, v)	__CAST(__CC_P, T, v)

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
			, (glob + (osz) * (isz + 1))		/** prm_in */
			, (v_predict).m_ret
			, glob					/** weight */
			, (glob + (osz) * (isz))[oidx]		/** Bias */
			, ACT
			, oidx
			, isz
			);

	glob[isz * osz + osz + isz + oidx] = (v_predict).m_ret;
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
	ae2f_float_t		delta;

	if(iidx == 0) {
		__ae2f_AnnSlpPredictOne_imp(
				v_predict
				, (glob + (osz) * (isz + 1))		/** prm_in */
				, (v_predict).m_ret			/** result: output */
				, glob					/** weight */
				, (glob + (osz) * (isz))[oidx]		/** Bias */
				, ACT
				, oidx
				, isz
				);

		/** output */
		glob[isz * osz + osz + isz + oidx] = loc[oidx] = (v_predict).m_ret;

		__ae2f_AnnSlpFetchDeltaOne_imp(
				loc						/** out */
				, glob + osz * isz + osz + isz + osz + oidx	/** out_desired */ 
				, ACT_DERIV
				, LOSS_DERIV
				, delta /** retdelta */
				, oidx
				, osz
				);

		/** delta */
		glob[isz * osz + osz + isz + osz + osz + oidx] = delta;


		__ae2f_AnnSlpFollowOneB_imp(
				glob[osz * isz] /** r_bias */
				, delta /** delta */
				, lr.m_bias
				);
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	__ae2f_AnnSlpFollowOneW_imp(
			(glob + osz * isz + osz)[iidx] /** inp */
			, (loc[oidx]) /** delta */
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
		, iidx = get_local_id(0)
		, isz = get_local_size(0)
		;

	ae2f_float_t	delta;

	if(iidx == 0) {
		__ae2f_AnnSlpFetchDeltaOne_imp(
				glob + osz * isz + osz + isz + oidx /** out */
				, glob + osz * isz + osz + isz + osz + osz + oidx /** out_desired */ 
				, ACT_DERIV
				, LOSS_DERIV
				, delta /** retdelta */
				, oidx
				, osz
				);

		/** delta */
		glob[osz * isz + osz + isz + osz + oidx] = delta;

		__ae2f_AnnSlpFollowOneB_imp(
				glob[osz * isz] /** r_bias */
				, delta /** delta */
				, lr.m_bias
				);
	}


	barrier(CLK_LOCAL_MEM_FENCE);

	__ae2f_AnnSlpFollowOneW_imp(
			(glob + osz * isz + osz)[iidx] /** inp */
			, delta /** delta */
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
		, iidx = get_local_id(0)
		, isz = get_local_size(0)
		;

	__ae2f_AnnSlpFollowOneW_imp(
			(glob + osz * isz + osz)[iidx] /** inp */
			, (glob[osz * isz + osz + isz + oidx]) /** delta */
			, glob /** weight */
			, lr.m_weight
			, isz
			, iidx
			, osz
			, oidx
			);

	if(iidx == 0) {
		__ae2f_AnnSlpFollowOneB_imp(
				glob[osz * isz] /** r_bias */
				, glob[osz * isz + osz + isz + oidx] /** delta */
				, lr.m_bias
				);
	}
}
