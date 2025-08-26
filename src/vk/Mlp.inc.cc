#include "ae2f/errGlob.h"
#include <vulkan/vulkan_core.h>
#ifndef ae2fVK_Ann_Mlp_h

#if __ae2f_MACRO_GENERATED
#define ae2fVK_Ann_Mlp_h
#endif

#include <ae2fVK/Ann/Mlp.h>
#include <ae2fVK/Ann/Mlp.core.h>
#include <assert.h>

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#include <ae2fVK/Ann/Mlp.auto.h>
#endif

#if __ae2f_MACRO_GENERATED



#endif

/**
 * @brief
 * Make MLP with OpenCL compute shader.
 *
 * @details
 * The shader itself is incomplete. \n
 * For using it you will need to define three macros as function. \n
 * Below is the required snippet.
 *
 * #define ACT(const ae2f_float_t) -> ae2f_float_t \n
 * #define ACT_DERIV(const ae2f_float_t) -> ae2f_float_t \n
 * #define LOSS_DERIV( 
 * 		ae2f_float_t* retval
 * 		const ae2f_float_t* const out
 * 		, const ae2f_float_t* const out_desired
 * 		, const size_t		index
 * 		, const size_t		length_out_goal
 * 		);
 *
 * @param vkcldeclaration
 * @param vkcldefinition
 * */
ae2f_MAC() _ae2fVK_AnnMlpMk_imp(
		ae2fVK_AnnMlpMk_t	v_mk

		, ae2f_float_t* const iv_weight_opt
		, ae2f_float_t* const iv_bias_opt
		, ae2f_float_t* const iv_cache_opt

		, const uint32_t	i_len_count
		, const uint32_t* const	i_len
		, const uint32_t* const	i_len_swap

		, ae2f_opt const size_t	i_prm_offset
		, ae2f_opt const size_t	i_extra

		, ae2f_opt ae2f_AnnAct_t** const i_act
		, ae2f_opt ae2f_AnnAct_t** const i_actderiv

		, ae2f_AnnLoss_t* const i_lossderiv

		, ae2f_float_t i_learningrate
		, ae2f_float_t i_learningrate_bias

		, const VkDevice					i_vkdev
		, const VkPhysicalDeviceMemoryProperties		i_vkmemprops
		, ae2f_opt VkAllocationCallbacks* const			iv_vkalloccalls,

		ae2f_opt const char* const				i_vkcldeclaration
		, ae2f_opt const char* const				i_vkcldefinition
) 
{
	(v_mk).m_reterr = ae2f_errGlob_OK;
	assert(i_len_count > 2);

	/** __ae2f_AnnMlpMk_imp(
			v_mk
			, i_len_count
			, i_len
			, szswap_opt
			, act
			, actderiv
			, lossderiv
			, deltastream
			, outcache
			, weight
			, bias
			, learningrate
			, learningrate_bias
			, offset
			, extra
			); */
}

#endif
