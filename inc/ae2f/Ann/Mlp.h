#undef	__ae2f_MACRO_GENERATED
#define	__ae2f_MACRO_GENERATED	1
#ifndef ae2f_Ann_Mlp_h
#define ae2f_Ann_Mlp_h

#include "ae2f/Cast.h"
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1
#include "ae2f/Float.auto.h"
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1
#include <ae2f/Ann/Slp.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1
#include <ae2f/Pack/Beg.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1

#include <ae2f/Macro.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1

ae2f_structdef(struct, ae2f_AnnMlp_t) 
{
	size_t	m_depth;

	/** @brief possible greatest output size. */
	size_t	m_outc;
};

ae2f_structdef_n(struct, ae2f_AnnMlpPredict_t, ae2f_AnnMlpPredictStream_t) 
{
	size_t		m_inc, m_outc, m_i, m_j, m_k, m_depth, m_outc_max;
	ae2f_float_t	m_ret;
};

ae2f_structdef(struct, ae2f_AnnMlpHidDeltaSingle_t) {
	/** 
	 * @brief delta seed, not delta. \n
	 * Use __ae2f_AnnPropagate to make delta.
	 * */
	ae2f_float_t	m_ret;
	size_t		m_outc, m_i;
};

ae2f_structdef(union, ae2f_AnnMlpPropagateAllStack_t) {
	size_t m_send;
};

ae2f_structdef(struct, ae2f_AnnMlpPropagateAll_t) {
	size_t				m_i, m_j, m_inc, m_outc;
	size_t				m_k, m_depth;
	size_t				m_pg_weight;
	size_t				m_pg_out;
	ae2f_AnnMlpPropagateAllStack_t	m_stack;
	ae2f_AnnSlp_t			m_slp_nxt;
	ae2f_float_t			m_ret;
}; 

#include <ae2f/Pack/End.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1

#endif


#if !defined(ae2f_Ann_Mlp_c)
#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Mlp.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1
#endif

#define ae2f_Ann_Mlp_c

#define __ae2f_AnnMlpPredictStream_imp( \
	/** tparam */ \
		 \
 \
	/** param */ \
		/*    ,  ae2f_AnnMlpPredict_t    */ v_predict, \
		/*   const  ae2f_AnnMlp_t     */ mlp, \
		/*   constae2f_float_t*  const    */ inp, \
		/*  ae2f_float_t*   const     */ out, \
		/*   constsize_t*   const    */ sz, \
		/*   constae2f_float_t*  const    */ weight, \
		/*   constae2f_float_t*  const    */ bias, \
		/*  ae2f_float_t*   const     */ outcache, \
		/*  ae2f_AnnAct_t*  const*  const    */ act_opt \
) \
{ \
	(v_predict).m_depth = (mlp).m_depth - 1; \
	(v_predict).m_outc_max = (mlp).m_outc; \
 \
	(v_predict).m_inc = (sz)[0]; \
	(v_predict).m_outc = (sz)[1]; \
 \
	if((act_opt)[0]) { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, inp \
				, outcache \
				, weight \
				, bias \
				, (act_opt)[0] \
				); \
	} else { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, inp \
				, outcache \
				, weight \
				, bias \
				, \
				); \
	} \
 \
	for( \
			(v_predict).m_k = 2;  \
			(v_predict).m_k < (v_predict).m_depth;  \
			++(v_predict).m_k \
	   ) \
	{ \
		(v_predict).m_inc = 	(v_predict).m_outc; \
		(v_predict).m_outc =	(sz)[(v_predict).m_k]; \
 \
		if((act_opt)[(v_predict).m_k - 2]) { \
			__ae2f_AnnSlpPredict_imp( \
					(v_predict) \
					, (v_predict) \
					, (outcache) + ((((v_predict).m_k) - 2) * (v_predict).m_outc_max) \
					, (outcache) + ((((v_predict).m_k) - 1) * (v_predict).m_outc_max) \
					, (weight) + ((v_predict).m_k - 2) * (v_predict).m_outc_max * (v_predict).m_outc_max \
					, (bias) + ((v_predict).m_k - 2) * (v_predict).m_outc_max \
					, (act_opt)[(v_predict).m_k - 2] \
					); \
		} else { \
			__ae2f_AnnSlpPredict_imp( \
					(v_predict) \
					, (v_predict) \
					, (outcache) + ((((v_predict).m_k) - 2) * (v_predict).m_outc_max) \
					, (outcache) + ((((v_predict).m_k) - 1) * (v_predict).m_outc_max) \
					, (weight) + ((v_predict).m_k - 2) * (v_predict).m_outc_max * (v_predict).m_outc_max \
					, (bias) + ((v_predict).m_k - 2) * (v_predict).m_outc_max \
					,  \
					); \
		} \
	} \
 \
	(v_predict).m_inc = 	(v_predict).m_outc; \
	(v_predict).m_outc =	(sz)[(v_predict).m_k]; \
 \
	if((act_opt)[(v_predict).m_k - 2]) { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, (outcache) + ((((v_predict).m_k) - 2) * (v_predict).m_outc_max) \
				, (outcache) + ((((v_predict).m_k) - 1) * (v_predict).m_outc_max) \
				, (weight) + ((v_predict).m_k - 2) * (v_predict).m_outc_max * (v_predict).m_outc_max \
				, (bias) + ((v_predict).m_k - 2) * (v_predict).m_outc_max \
				, (act_opt)[(v_predict).m_k - 2] \
				); \
	} else { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, (outcache) + ((((v_predict).m_k) - 2) * (v_predict).m_outc_max) \
				, (outcache) + ((((v_predict).m_k) - 1) * (v_predict).m_outc_max) \
				, (weight) + ((v_predict).m_k - 2) * (v_predict).m_outc_max * (v_predict).m_outc_max \
				, (bias) + ((v_predict).m_k - 2) * (v_predict).m_outc_max \
				,  \
				); \
	} \
}

#define __ae2f_AnnMlpPredict_imp( \
	/** tparam */ \
		 \
 \
	/** param */ \
		/*    ,  ae2f_AnnMlpPredict_t    */ v_predict, \
		/*   const  ae2f_AnnMlp_t     */ mlp, \
		/*   constae2f_float_t*  const    */ inp, \
		/*  ae2f_float_t*   const     */ out, \
		/*   constsize_t*   const    */ sz, \
		/*   constae2f_float_t*  const    */ weight, \
		/*   constae2f_float_t*  const    */ bias, \
		/*  ae2f_float_t*   const     */ outcache, \
		/*  ae2f_AnnAct_t*  const*  const    */ act_opt \
) \
{ \
	(v_predict).m_depth = (mlp).m_depth - 1; \
	(v_predict).m_outc_max = (mlp).m_outc; \
 \
	(v_predict).m_inc = (sz)[0]; \
	(v_predict).m_outc = (sz)[1]; \
 \
	if((act_opt)[0]) { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, inp \
				, outcache \
				, weight \
				, bias \
				, (act_opt)[0] \
				); \
	} else { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, inp \
				, outcache \
				, weight \
				, bias \
				, \
				); \
	} \
 \
	for( \
			(v_predict).m_k = 2;  \
			(v_predict).m_k < (v_predict).m_depth;  \
			++(v_predict).m_k \
	   ) \
	{ \
		(v_predict).m_inc = 	(v_predict).m_outc; \
		(v_predict).m_outc =	(sz)[(v_predict).m_k]; \
 \
		if((act_opt)[(v_predict).m_k & 1]) { \
			__ae2f_AnnSlpPredict_imp( \
					(v_predict) \
					, (v_predict) \
					, (outcache) + ((((v_predict).m_k) & 1) * (v_predict).m_outc_max) \
					, (outcache) + ((((v_predict).m_k) ^ 1 & 1) * (v_predict).m_outc_max) \
					, (weight) + ((v_predict).m_k & 1) * (v_predict).m_outc_max * (v_predict).m_outc_max \
					, (bias) + ((v_predict).m_k & 1) * (v_predict).m_outc_max \
					, (act_opt)[(v_predict).m_k & 1] \
					); \
		} else { \
			__ae2f_AnnSlpPredict_imp( \
					(v_predict) \
					, (v_predict) \
					, (outcache) + ((((v_predict).m_k) & 1) * (v_predict).m_outc_max) \
					, (outcache) + ((((v_predict).m_k) ^ 1 & 1) * (v_predict).m_outc_max) \
					, (weight) + ((v_predict).m_k & 1) * (v_predict).m_outc_max * (v_predict).m_outc_max \
					, (bias) + ((v_predict).m_k & 1) * (v_predict).m_outc_max \
					,  \
					); \
		} \
	} \
 \
	(v_predict).m_inc = 	(v_predict).m_outc; \
	(v_predict).m_outc =	(sz)[(v_predict).m_k]; \
 \
	if((act_opt)[(v_predict).m_k & 1]) { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, (outcache) + ((((v_predict).m_k) & 1) * (v_predict).m_outc_max) \
				, (outcache) + ((((v_predict).m_k) ^ 1 & 1) * (v_predict).m_outc_max) \
				, (weight) + ((v_predict).m_k & 1) * (v_predict).m_outc_max * (v_predict).m_outc_max \
				, (bias) + ((v_predict).m_k & 1) * (v_predict).m_outc_max \
				, (act_opt)[(v_predict).m_k & 1] \
				); \
	} else { \
		__ae2f_AnnSlpPredict_imp( \
				(v_predict) \
				, (v_predict) \
				, (outcache) + ((((v_predict).m_k) & 1) * (v_predict).m_outc_max) \
				, (outcache) + ((((v_predict).m_k) ^ 1 & 1) * (v_predict).m_outc_max) \
				, (weight) + ((v_predict).m_k & 1) * (v_predict).m_outc_max * (v_predict).m_outc_max \
				, (bias) + ((v_predict).m_k & 1) * (v_predict).m_outc_max \
				,  \
				); \
	} \
}

	


#define __ae2f_AnnMlpHidDeltaSingle_imp( \
	/** tparam */ \
		 \
 \
	/** param */ \
		/*    , ae2f_AnnMlpHidDeltaSingle_t    */ v_single, \
		/*   const  ae2f_AnnSlp_t    */ slp, \
		/*   constae2f_float_t*  const    */ weight, \
		/*   constae2f_float_t*  const    */ delta, \
		/*   const   size_t    */ oidx_then \
) \
{ \
	(v_single).m_outc = (slp).m_outc; \
	(v_single).m_ret = 0; \
 \
	for((v_single).m_i = 0; (v_single).m_i < (v_single).m_outc; ++(v_single).m_i) \
	{ \
		(v_single).m_ret +=  \
			((weight) + (slp).m_inc * (v_single).m_i)[oidx_then] * (delta)[(v_single).m_i]; \
	} \
}

/** @brief delta to delta */
#define __ae2f_AnnMlpPropagate_imp( \
	/** tparam */ \
		 \
 \
	/** param */ \
		/*    ,    size_t    */ v_send, \
		/*   const  ae2f_AnnSlp_t    */ slp_then, \
		/*  ae2f_float_t*   const     */ retdelta_then, \
		/*   constae2f_float_t*  const    */ deltaseed, \
		/*     ae2f_AnnAct_t     */ actderiv_then, \
		/*   constae2f_float_t*  const    */ inp \
) \
{ \
	for((v_send) = (slp_then).m_outc; (v_send)--;) { \
		retdelta_then[v_send] = actderiv_then(inp[v_send]) * (deltaseed)[v_send]; \
	} \
}

#include <stdio.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1

#define __ae2f_AnnMlpPropagateAll_imp( \
	/** tparam */ \
		 \
 \
	/** param */ \
		/*    , ae2f_AnnMlpPropagateAll_t    */ v_follow, \
		/*   const  ae2f_AnnMlp_t     */ mlp, \
		/*   constae2f_float_t*  const    */ inp, \
		/*   constae2f_float_t*  const    */ out, \
		/*   constae2f_float_t*  const    */ delta, \
		/*   constsize_t*   const     */ lenv, \
		/*   constae2f_float_t*  const     */ outstream, \
		/*  ae2f_float_t*   const    */ deltacache, \
		/*  ae2f_float_t*   const    */ weight, \
		/*  ae2f_float_t*   const     */ bias, \
		/*   const  ae2f_float_t    */ learningrate, \
		/*   const  ae2f_float_t     */ learningrate_bias, \
		/*  ae2f_AnnAct_t*  const*  const    */ actderiv \
) \
{ \
	(v_follow).m_depth = (mlp).m_depth - 1; \
	(v_follow).m_inc = (lenv)[(v_follow).m_depth - 2]; \
	(v_follow).m_outc = (lenv)[(v_follow).m_depth - 1]; \
 \
	(v_follow).m_slp_nxt.m_inc = (v_follow).m_outc; \
	(v_follow).m_pg_weight = (v_follow).m_pg_out = (mlp).m_outc; \
	(v_follow).m_pg_weight *= (v_follow).m_pg_out; \
 \
	/** field update */ \
	__ae2f_AnnSlpFollow_imp( \
			v_follow \
			, v_follow \
			, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_depth - 2)) \
			, (delta) \
			, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_depth - 1)) \
			, (bias) + ((v_follow).m_pg_out * ((v_follow).m_depth - 1)) \
			, learningrate \
			, learningrate_bias \
			); \
 \
	/** delta update */ \
	for((v_follow).m_j = (v_follow).m_outc; (v_follow).m_j--;) { \
		__ae2f_AnnMlpHidDeltaSingle_imp( \
				(v_follow) \
				, (v_follow) \
				, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_depth - 1)) \
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_depth - 1))) \
				, (v_follow).m_j \
				); \
 \
		((deltacache) + (v_follow).m_pg_out * ((v_follow).m_depth - 1))[(v_follow).m_j] \
			= (v_follow).m_ret; \
	} \
 \
	if((actderiv)[(v_follow).m_depth - 2]) { \
		__ae2f_AnnMlpPropagate_imp( \
				(v_follow).m_stack.m_send \
				, (v_follow) \
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_depth - 2))) \
				, (delta) \
				, (actderiv)[(v_follow).m_depth - 2] \
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_depth - 2)) \
				); \
	} else { \
		__ae2f_AnnMlpPropagate_imp( \
				(v_follow).m_stack.m_send \
				, (v_follow) \
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_depth - 2))) \
				, (delta) \
				, /** actderiv */ \
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_depth - 2)) \
				); \
	} \
 \
	(v_follow).m_k = (v_follow).m_depth - 1; \
	/** m_k is index for next */ \
	for(; (v_follow).m_k-- && (v_follow).m_k;) { \
		puts("hi"); \
 \
		(v_follow).m_slp_nxt.m_outc = (v_follow).m_slp_nxt.m_inc; \
		(v_follow).m_slp_nxt.m_inc = (v_follow).m_outc = (v_follow).m_inc; \
		(v_follow).m_inc = (lenv)[(v_follow).m_k - 1]; \
 \
		for((v_follow).m_j = (v_follow).m_outc; (v_follow).m_j--;) { \
			__ae2f_AnnMlpHidDeltaSingle_imp( \
					(v_follow) \
					, (v_follow) \
					, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_k)) \
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k))) \
					, (v_follow).m_j \
					); \
 \
			((deltacache) + (v_follow).m_pg_out * (v_follow).m_k)[(v_follow).m_j] \
				= (v_follow).m_ret; \
		} \
 \
		if((actderiv)[(v_follow).m_k - 1]) { \
			__ae2f_AnnMlpPropagate_imp( \
					(v_follow).m_stack.m_send \
					, (v_follow) \
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1))) \
					, ((deltacache) + (v_follow).m_pg_out * (v_follow).m_k) \
					, (actderiv)[(v_follow).m_k - 1] \
					, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1)) \
					); \
		} else { \
			__ae2f_AnnMlpPropagate_imp( \
					(v_follow).m_stack.m_send \
					, (v_follow) \
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1))) \
					, ((deltacache) + (v_follow).m_pg_out * (v_follow).m_k) \
					,  \
					, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1)) \
					); \
		} \
 \
		__ae2f_AnnSlpFollow_imp( \
				(v_follow) \
				, (v_follow) \
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1)) \
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1))) \
				, ((weight) + ((v_follow).m_pg_weight * ((v_follow).m_k - 1))) \
				, ((bias) + ((v_follow).m_pg_out * (v_follow).m_k - 1)) \
				, learningrate \
				, learningrate_bias \
				); \
	} \
 \
	(v_follow).m_slp_nxt.m_outc = (v_follow).m_slp_nxt.m_inc; \
	(v_follow).m_slp_nxt.m_inc = (v_follow).m_outc = (v_follow).m_inc; \
	(v_follow).m_inc = (lenv)[0]; \
 \
	/** last */ \
	for((v_follow).m_j = (v_follow).m_outc; (v_follow).m_j--;) { \
		__ae2f_AnnMlpHidDeltaSingle_imp( \
				(v_follow) \
				, (v_follow) \
				, ((weight) + ((v_follow).m_pg_weight)) \
				, ((deltacache) + ((v_follow).m_pg_out)) \
				, (v_follow).m_j \
				); \
 \
		((deltacache) + (v_follow).m_pg_out)[(v_follow).m_j] \
			= (v_follow).m_ret; \
	} \
 \
	if((actderiv)[0]) { \
		__ae2f_AnnMlpPropagate_imp( \
				(v_follow).m_stack.m_send \
				, (v_follow) \
				, ((deltacache)) \
				, ((deltacache) + (v_follow).m_pg_out) \
				, (*(actderiv)) \
				, ((inp)) \
				); \
	} else { \
		__ae2f_AnnMlpPropagate_imp( \
				(v_follow).m_stack.m_send \
				, (v_follow) \
				, ((deltacache)) \
				, ((deltacache) + (v_follow).m_pg_out) \
				,  \
				, ((inp)) \
				); \
	} \
 \
	__ae2f_AnnSlpFollow_imp( \
			(v_follow) \
			, (v_follow) \
			, (inp) \
			, ((deltacache)) \
			, ((weight)) \
			, ((bias)) \
			, learningrate \
			, learningrate_bias \
			); \
}

#endif

#undef	__ae2f_MACRO_GENERATED

#define	__ae2f_MACRO_GENERATED	0

