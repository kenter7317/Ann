#ifndef ae2f_Ann_Mhattn_auto_h
#define ae2f_Ann_Mhattn_auto_h

#include <ae2f/Ann/Mhattn.h>

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#endif

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#endif

/**
 * @brief
 * 
 * @param ret_attnw	(m_headc, prm_seqlen, prm_seqlen)
 * @param ret_attno	(prm_seqlen, m_mdldist)
 * @param ret_out	(prm_seqlen, m_mdldist)
 *
 * @param ref_qcache	(prm_seqlen, m_mdldist)
 * @param ref_kcache	(prm_seqlen, m_mdldist)
 * @param ref_vcache	(prm_seqlen, m_mdldist)
 *
 * @param prm_qry	(prm_seqlen, m_mdldist)
 * @param prm_key	(prm_seqlen, m_mdldist)
 * @param prm_val	(prm_seqlen, m_mdldist)
 *
 * @param prm_wqry	(prm_mdldist, prm_mdldist)
 * @param prm_wkey	(prm_mdldist, prm_mdldist)
 * @param prm_wval	(prm_mdldist, prm_mdldist)
 * @param prm_wout	(prm_mdldist, prm_mdldist)
 * */
ae2f_MAC() ae2f_AnnMhattnFwd_imp(
		ae2f_AnnMhattnFwd_t			ref_mem,

		ae2f_float_t* const			ref_qcache,
		ae2f_float_t* const			ref_kcache,
		ae2f_float_t* const			ref_vcache,
		ae2f_float_t* const			ref_attnw_cache,

		const ae2f_AnnMhattn_t			prm_mhattn,
		const ae2f_float_t* const		prm_wqry, 
		const ae2f_float_t* const		prm_wkey,
		const ae2f_float_t* const		prm_wval,
		const ae2f_float_t* const		prm_wout,
		const ae2f_float_t* const		prm_qry,
		const ae2f_float_t* const		prm_key,
		const ae2f_float_t* const		prm_val,
		ae2f_opt const ae2f_float_t* const	prm_mask_opt,

		const size_t				prm_seqlen,

		ae2f_AnnAct_t				prm_act,

		ae2f_float_t* const			ret_out,
		ae2f_float_t* const			ret_attnw,
		ae2f_float_t* const			ret_attno
)
{
	{
		assert(ref_kcache && "ae2f_AnnMhattnFwd_imp");
		assert(ref_qcache && "ae2f_AnnMhattnFwd_imp");
		assert(ref_vcache && "ae2f_AnnMhattnFwd_imp");
		assert(ref_attnw_cache && "ae2f_AnnMhattnFwd_imp");

		assert(prm_wqry && "ae2f_AnnMhattnFwd_imp");
		assert(prm_wkey && "ae2f_AnnMhattnFwd_imp");
		assert(prm_wval && "ae2f_AnnMhattnFwd_imp");
		assert(prm_wout && "ae2f_AnnMhattnFwd_imp");

		assert(prm_qry && "ae2f_AnnMhattnFwd_imp");
		assert(prm_key && "ae2f_AnnMhattnFwd_imp");
		assert(prm_val && "ae2f_AnnMhattnFwd_imp");

		assert(ret_out && "ae2f_AnnMhattnFwd_imp");
		assert(ret_attnw && "ae2f_AnnMhattnFwd_imp");
		assert(ret_attno && "ae2f_AnnMhattnFwd_imp");
	}

	(ref_mem).m_i = (prm_seqlen) /* i */ * ((prm_mhattn).m_mdldist) /* k */; 

	/** 
	 * Matrix multiplication.
	 * */
	while((ref_mem).m_i--) {
		(ref_mem).m_U0.m_S0.m_k = 0;
		(ref_mem).m_U0.m_S0.m_q = 0;
		(ref_mem).m_U0.m_S0.m_v = 0;

		for((ref_mem).m_j = (prm_mhattn).m_mdldist /* j */; (ref_mem).m_j--;) {
			(ref_mem).m_U0.m_S0.m_q
				+= ae2f_AnnMhattnFwdSeqConvOne_imp(
						prm_qry
						, prm_wqry
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_k % (prm_mhattn).m_mdldist
						);

			(ref_mem).m_U0.m_S0.m_k
				+= ae2f_AnnMhattnFwdSeqConvOne_imp(
						prm_key
						, prm_wkey
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_k % (prm_mhattn).m_mdldist
						);

			(ref_mem).m_U0.m_S0.m_v
				+= ae2f_AnnMhattnFwdSeqConvOne_imp(
						prm_val
						, prm_wval
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_k % (prm_mhattn).m_mdldist
						);
		}

		(ref_qcache)[(ref_mem).m_i] = (ref_mem).m_U0.m_S0.m_q;
		(ref_kcache)[(ref_mem).m_i] = (ref_mem).m_U0.m_S0.m_k;
		(ref_vcache)[(ref_mem).m_i] = (ref_mem).m_U0.m_S0.m_v;
	}

	/**
	 * @brief
	 * cache could be read as (m_headc, prm_seqlen, kdist)
	 * */
	for((ref_mem).m_i = (prm_mhattn).m_headc; (ref_mem).m_i--;) {
		(ref_mem).m_j = (prm_seqlen) /* i */ * (prm_seqlen) /* k */;
		/** 
		 * q * Transpose(k) goes to (prm_seqlen, prm_seqlen)
		 * */
		while( (ref_mem).m_j--) {
			(ref_mem).m_U0.m_one = 0;
			(ref_mem).m_k = ae2f_AnnMhattnKDist(prm_mhattn) /* j */;

			while((ref_mem).m_k--) {
				/** Since it is transposing, index must be (i, j) * (k, j) */
				(ref_mem).m_U0.m_one
					+= (ref_qcache)[ae2f_AnnMhattnHeadSplit_imp(
							prm_mhattn
							, prm_seqlen
							, (ref_mem).m_i
							, (ref_mem).m_j / (prm_seqlen)
							, (ref_mem).m_k)
					]
					* (ref_kcache)[ae2f_AnnMhattnHeadSplit_imp(/**/
							prm_mhattn
							, prm_seqlen
							, (ref_mem).m_i
							, (ref_mem).m_j % (prm_seqlen)
							, (ref_mem).m_k)
					];
			}

			(ref_attnw_cache)[ae2f_AnnUtilIdx3(
					(ref_mem).m_i
					, (prm_mhattn).m_headc
					, 0, (prm_seqlen)
					, 0, (prm_seqlen)
					) + (ref_mem).m_j]

				= 
				((ref_mem).m_U0.m_one / sqrt(
					ae2f_static_cast(
						ae2f_float_t
						, ae2f_AnnMhattnKDist(prm_mhattn)))
				)
				+ ((prm_mask_opt) ? (prm_mask_opt)[(ref_mem).m_j] : 0.);

				prm_act(
						&(ret_attnw)[ae2f_AnnUtilIdx3(
							(ref_mem).m_i
							, (prm_mhattn).m_headc
							, 0, (prm_seqlen)
							, 0, (prm_seqlen)
							) + (ref_mem).m_j]

						, &(ref_attnw_cache)[ae2f_AnnUtilIdx3(
							(ref_mem.m_i), (prm_mhattn).m_headc
							, (ref_mem).m_j / (prm_seqlen), (prm_seqlen)
							, 0, prm_seqlen
							)]

						, (ref_mem).m_j % (prm_seqlen)
						, (prm_seqlen)
				       );
		}

		(ref_mem).m_j = (prm_seqlen) /* i */ * ae2f_AnnMhattnKDist(prm_mhattn) /* k */;
		while((ref_mem).m_j--)
		{
			(ref_mem).m_U0.m_one = 0;
			for((ref_mem).m_k = (prm_seqlen) /* j */; (ref_mem).m_k--;)
			{
				(ref_mem).m_U0.m_one
					+= (ret_attnw)[ae2f_AnnUtilIdx3(
							(ref_mem).m_i, (prm_mhattn).m_headc
							, (ref_mem).m_j / ae2f_AnnMhattnKDist(prm_mhattn), prm_seqlen
							, (ref_mem).m_k, prm_seqlen
							)] * /**/
					(ref_vcache)[ae2f_AnnMhattnHeadSplit_imp(
							prm_mhattn
							, prm_seqlen
							, (ref_mem).m_i
							, (ref_mem).m_k
							, (ref_mem).m_j % ae2f_AnnMhattnKDist(prm_mhattn)
							)];
			}

			(ret_attno)[ae2f_AnnMhattnHeadSplit_imp(
					prm_mhattn
					, prm_seqlen
					, (ref_mem).m_i
					, 0, 0
					) + (ref_mem).m_j] = (ref_mem).m_U0.m_one;
		}
	}

	(ref_mem).m_i = (prm_seqlen) /* i */ * (prm_mhattn).m_mdldist /* k */;
	while((ref_mem).m_i--) {
		(ref_mem).m_j = ((prm_mhattn).m_mdldist) /* j */;
		(ref_mem).m_U0.m_one = 0;

		while((ref_mem).m_j--) {
			(ref_mem).m_U0.m_one +=
				(ret_attno)[ae2f_AnnUtilIdx2(
						(ref_mem).m_i / (prm_mhattn).m_mdldist, (prm_seqlen)
						, (ref_mem).m_j, (prm_mhattn).m_mdldist
						)] *
				(prm_wout)[ae2f_AnnUtilIdx2(
						(ref_mem).m_j, (prm_mhattn).m_mdldist
						, (ref_mem).m_i % (prm_mhattn).m_mdldist
						, (prm_mhattn).m_mdldist)];
		}

		(ret_out)[(ref_mem).m_i] = (ref_mem).m_U0.m_one;
	}
}

ae2f_MAC() ae2f_AnnMhattnBwd_imp(
		const ae2f_AnnMhattn_t			prm_mhattn,
		ae2f_float_t* const			ret_attnw,
		ae2f_float_t* const			ret_attno
		) 
{
}

#endif
