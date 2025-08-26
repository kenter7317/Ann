#include "ae2f/Cast.h"
#include "ae2f/errGlob.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>
#ifndef ae2fVK_Ann_Mlp_h

#if __ae2f_MACRO_GENERATED
#define ae2fVK_Ann_Mlp_h
#endif

#include <ae2fVK/Ann/Mlp.h>
#include <assert.h>

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#include <ae2fVK/Ann/Mlp.auto.h>
#endif

#if !__ae2f_MACRO_GENERATED || !ae2f_MAC_BUILD
#include <stdlib.h>
#include <limits.h>
#include <ae2fVK/Ann/Slp.auto.h>
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
		ae2fVK_AnnMlpMk_t	v_mk,

		ae2f_opt ae2f_float_t* const iv_weight_opt,
		ae2f_opt ae2f_float_t* const iv_bias_opt,
		ae2f_opt ae2f_float_t* const iv_outstream_opt,
		ae2f_opt ae2f_float_t* const iv_deltastream_opt,

		const uint32_t	i_len_count,
		const size_t* const	i_len,
		ae2f_opt size_t* const i_len_swap,

		ae2f_opt const size_t	i_prm_offset,
		ae2f_opt const size_t	i_extra,

		ae2f_opt ae2f_AnnAct_t** const i_act,
		ae2f_opt ae2f_AnnAct_t** const i_actderiv,

		ae2f_AnnLoss_t* const i_lossderiv,

		ae2f_float_t i_learningrate,
		ae2f_float_t i_learningrate_bias,

		const VkDevice					i_vkdev,
		const VkPhysicalDeviceMemoryProperties		i_vkmemprops,
		const VkQueue					i_vkqueue,

		ae2f_opt VkAllocationCallbacks* const		iv_vkalloccalls,

		ae2f_opt const char* const			i_vkcldeclaration,
		ae2f_opt const char* const			i_vkcldefinition
		)
{
	(v_mk).m_ret.m_err = ae2f_errGlob_OK;
	assert(i_len_count > 2);

	do {
		(v_mk).m_U1.m_i = (i_len_count);
		while((v_mk).m_U1.m_i-- && (i_len)[(v_mk).m_U1.m_i] <= UINT32_MAX) {}
		if((v_mk).m_U1.m_i != ae2f_static_cast(size_t, -1)) {
			assert(!"`i_len` exceeds the expected size (4 bytes)");
			break;
		}

		__ae2f_AnnMlpMk_imp(
				(v_mk).m_U0.m_mk
				, i_len_count
				, i_len
				, i_len_swap
				, i_act
				, i_actderiv
				, i_lossderiv
				, iv_deltastream_opt
				, iv_outstream_opt
				, iv_weight_opt
				, iv_bias_opt
				, i_learningrate
				, i_learningrate_bias
				, ((i_prm_offset) + sizeof(ae2fVK_AnnMlp) - sizeof(ae2f_AnnMlp))
				, i_extra
				);

		if(!(v_mk).m_U0.m_mkswap.m_mkbase)
			break;

		(v_mk).m_U0.m_mkswap.m_mkbase->m_vkres = VK_SUCCESS;
		if((i_vkdev)) { (v_mk).m_ret.m_err |= ae2f_errGlob_PTR_IS_NULL; break; }
		(v_mk).m_U0.m_mkswap.m_mkbase->m_vkdev = (i_vkdev);
		(v_mk).m_U0.m_mkswap.m_mkbase->m_vkalloccalls = (iv_vkalloccalls);

		/** Global memory allocation */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break
				, (sizeof(uint32_t) * (v_mk).m_U0.m_mk.m_mkbase->m_depth)
				+ (sizeof(ae2f_float_t) * (v_mk).m_U0.m_mk.m_mkbase->m_outc * (
						((v_mk).m_U0.m_mk.m_mkbase->m_depth - 1) *
						((v_mk).m_U0.m_mk.m_mkbase->m_outc + 2)
						+ (v_mk).m_U0.m_mk.m_mkbase->m_depth
						))
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkres
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkglobbuf
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkglobdevmem /** r_vkdevmem */
				, (v_mk).m_ret.m_err /** v_errbit */
				, (v_mk).m_U1.m_vkbufcreatinfo /** v_vkbufcreatinfo */
				, (v_mk).m_U1.m_vkmemallocinfo
				, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT/** vkbufuseflags */
				, VK_SHARING_MODE_EXCLUSIVE
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkdev
				, (v_mk).m_U2.m_vkmemreqs
				, (iv_vkalloccalls)
				, (v_mk).m_U3.m_vkmemtypeidx
				, UCHAR_MAX
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				, i_vkmemprops
				);

		(v_mk).m_U2.m_i = (v_mk).m_U0.m_mk.m_mkbase->m_depth * sizeof(uint32_t);
		__ae2fVK_AnnMlpMapRanged_imp(
				(v_mk).m_U1.m_map /* v_map */
				, (v_mk).m_ret.m_err
				, *(v_mk).m_U0.m_mkswap.m_mkbase
				, i_vkqueue
				, 0
				, (v_mk).m_U2.m_i
				);

		while((v_mk).m_U2.m_i--) {
			ae2f_reinterpret_cast(uint32_t*, (v_mk).m_U1.m_map.m_map.m_v)[(v_mk).m_U2.m_i]
				= (i_len)[(v_mk).m_U2.m_i];
		}

		__ae2fVK_AnnMlpUnMapRanged_imp(
				(v_mk).m_U1.m_unmap
				, *(v_mk).m_U0.m_mkswap.m_mkbase
				, i_vkqueue
				, 0, ((v_mk).m_U0.m_mk.m_mkbase->m_depth * sizeof(uint32_t))
				);

		/** Local memory allocation */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break
				, ((
					(v_mk).m_U0.m_mk.m_mkbase->m_outc * 
					((v_mk).m_U0.m_mk.m_mkbase->m_depth + 2)
					) * sizeof(ae2f_float_t))
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkres
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vklocbuf
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vklocdevmem	/** r_vkdevmem */
				, (v_mk).m_ret.m_err					/** v_errbit */
				, (v_mk).m_U1.m_vkbufcreatinfo				/** v_vkbufcreatinfo */
				, (v_mk).m_U1.m_vkmemallocinfo
				, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT			/** vkbufuseflags */
				, VK_SHARING_MODE_EXCLUSIVE
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkdev
				, (v_mk).m_U2.m_vkmemreqs
				, (iv_vkalloccalls)
				, (v_mk).m_U3.m_vkmemtypeidx
				, UCHAR_MAX
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
				, i_vkmemprops
				);

		(v_mk).m_U1.m_vkdescsetlayoutbind[0].descriptorType
			= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		(v_mk).m_U1.m_vkdescsetlayoutbind[0].descriptorCount = 1;
		(v_mk).m_U1.m_vkdescsetlayoutbind[0].pImmutableSamplers = NULL;
		(v_mk).m_U1.m_vkdescsetlayoutbind[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		(v_mk).m_U1.m_vkdescsetlayoutbind[1]
		= (v_mk).m_U1.m_vkdescsetlayoutbind[0];

		(v_mk).m_U1.m_vkdescsetlayoutbind[0].binding = 0;
		(v_mk).m_U1.m_vkdescsetlayoutbind[1].binding = 1;

		(v_mk).m_U2.m_vkdescsetlayoutcreatinfo.sType
			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		(v_mk).m_U2.m_vkdescsetlayoutcreatinfo.pNext = NULL;
		(v_mk).m_U2.m_vkdescsetlayoutcreatinfo.bindingCount = 2;
		(v_mk).m_U2.m_vkdescsetlayoutcreatinfo.pBindings 
			= (v_mk).m_U1.m_vkdescsetlayoutbind;
		(v_mk).m_U2.m_vkdescsetlayoutcreatinfo.flags = 0;

		if(((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres = vkCreateDescriptorSetLayout(
						(i_vkdev)
						, &(v_mk).m_U2.m_vkdescsetlayoutcreatinfo
						, (iv_vkalloccalls)
						, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkdescsetlayout
						)) != VK_SUCCESS)
		{
			assert(!"Failed VkCreateDescriptorSetLayout");
			break;
		}

		if(!(v_mk).m_U0.m_mkswap.m_mkbase->m_vkdescsetlayout[0]) {
			assert(!"VkCreateDescriptorSetLayout went null");
			(v_mk).m_ret.m_err |= ae2f_errGlob_PTR_IS_NULL;
			break;
		}

		(v_mk).m_U1.m_vkpushconst.offset = 0;
		(v_mk).m_U1.m_vkpushconst.size = 0;
		(v_mk).m_U1.m_vkpushconst.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		(v_mk).m_U2.m_vkpipelayoutcreatinfo.flags = 0;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pNext = NULL;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pPushConstantRanges 
			= &(v_mk).m_U1.m_vkpushconst;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pushConstantRangeCount = 1;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.setLayoutCount = 1;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.sType
			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pSetLayouts
			= (v_mk).m_U0.m_mkswap.m_mkbase->m_vkdescsetlayout;		
	} while(0);

	do {
		if(!(v_mk).m_U0.m_mkswap.m_mkbase) {
			assert("Allocation has failed.");
			(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		if((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres != VK_SUCCESS) {
			assert(!"m_vkres was not VK_SUCCESS");
			(v_mk).m_ret.m_err |= ae2f_errGlob_NFOUND;
			break;
		}

		assert((v_mk).m_ret.m_err == ae2f_errGlob_OK);
	} while(0);
}

#endif
