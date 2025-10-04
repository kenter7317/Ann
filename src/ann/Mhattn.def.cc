#ifndef ae2f_Ann_Mhattn_auto_h
#define ae2f_Ann_Mhattn_auto_h

#include <ae2f/Ann/Mhattn.h>

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <assert.h>
#include <stdlib.h>
#endif

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#endif

/**
 * @brief
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
 * */
ae2f_MAC() ae2f_AnnMhattnForward_imp(
		ae2f_AnnMhattnForward_t			ref_mem,

		ae2f_float_t* const			ref_qcache,
		ae2f_float_t* const			ref_kcache,
		ae2f_float_t* const			ref_vcache,

		const ae2f_AnnMhattn_t			prm_mhattn,
		const ae2f_float_t* const		prm_wqry, 
		const ae2f_float_t* const		prm_wkey,
		const ae2f_float_t* const		prm_wval,
		const ae2f_float_t* const		prm_qry,
		const ae2f_float_t* const		prm_key,
		const ae2f_float_t* const		prm_val,
		ae2f_opt const ae2f_float_t* const	prm_mask_opt,

		const size_t				prm_seqlen,

		ae2f_float_t* const			ret_out,
		ae2f_float_t* const			ret_attnw,
		ae2f_float_t* const			ret_attno
)
{
	/** 
	 * Matrix multiplication.
	 * Output shall be 
	 * */
	for((ref_mem).m_i = (prm_seqlen) * ((prm_mhattn).m_mdldist); (ref_mem).m_i--;) {
		(ref_qcache)[(ref_mem).m_i] = 0;
		(ref_kcache)[(ref_mem).m_i] = 0;
		(ref_vcache)[(ref_mem).m_i] = 0;

		for((ref_mem).m_j = (prm_mhattn).m_mdldist; (ref_mem).m_j--;) {
			(ref_qcache)[(ref_mem).m_i]
				+= ae2f_AnnMhattnForwardSeqConvOne_imp(
						prm_qry
						, prm_wqry
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_k % (prm_mhattn).m_mdldist
						);

			(ref_kcache)[(ref_mem).m_i]
				+= ae2f_AnnMhattnForwardSeqConvOne_imp(
						prm_key
						, prm_wkey
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_k % (prm_mhattn).m_mdldist
						);

			(ref_vcache)[(ref_mem).m_i]
				+= ae2f_AnnMhattnForwardSeqConvOne_imp(
						prm_val
						, prm_wval
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_k % (prm_mhattn).m_mdldist
						);
		}
	}
}

#endif
