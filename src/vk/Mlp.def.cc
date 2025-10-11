#ifndef ae2fVK_Ann_Mlp_auto_h

#if __ae2f_MACRO_GENERATED
#define ae2fVK_Ann_Mlp_auto_h
#endif

#include <ae2fVK/Ann/Mlp.h>
#include <ae2fVK/Ann/Mlp.auto.h>
#include <ae2fVK/Ann/MlpSHADER.auto.h>
#include <ae2fVK/Ann/Slp.auto.h>

#if __ae2f_MACRO_GENERATED
#else
#include <ae2f/Macro.h>
typedef ae2f_float_t cllocfloat_t;
#endif

#if !__ae2f_MACRO_GENERATED || !ae2f_MAC_BUILD
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#endif 


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

		ae2f_opt VkAllocationCallbacks* const		iv_vkalloccalls,

		ae2f_opt const char* const			i_vkcldeclaration,
		ae2f_opt const char* const			i_vkcldefinition
		)
{
	__ae2fVK_AnnMlpMk_imp_V(
			ae2f_float_t, v_mk
			, iv_weight_opt, iv_bias_opt, iv_outstream_opt, iv_deltastream_opt
			, i_len_count, i_len, i_len_swap
			, i_prm_offset, i_extra
			, i_act, i_actderiv, i_lossderiv
			, i_learningrate, i_learningrate_bias
			, i_vkdev, i_vkmemprops, iv_vkalloccalls
			, i_vkcldeclaration, i_vkcldefinition
			);
}

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
ae2f_MAC(cllocfloat_t, ) _ae2fVK_AnnMlpMk_imp_V(
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
			(v_mk).m_ret.m_err |= ae2f_errGlob_IMP_NOT_FOUND;
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

		unless((v_mk).m_U0.m_mkswap.m_mkbase) {
			assert(!"__ae2f_AnnMlpMk_imp went null");
			(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		if((v_mk).m_ret.m_err) {
			assert(!"__ae2f_AnnMlpMk_imp has failed.");
			break;
		}

		(v_mk).m_U0.m_mkswap.m_mkbase->m_vkres = VK_SUCCESS;
		unless((i_vkdev)) {
			assert(!"i_vkdev is null");
			(v_mk).m_ret.m_err |= ae2f_errGlob_PTR_IS_NULL; break; 
		}
		(v_mk).m_U0.m_mkswap.m_mkbase->m_vkdev = (i_vkdev);
		(v_mk).m_U0.m_mkswap.m_mkbase->m_vkalloccalls = (iv_vkalloccalls);

		assert((v_mk).m_U0.m_mk.m_outc == (v_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_outc);

		/** Global memory allocation */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break
				, __ae2fVK_AnnMlpGlobMemSz(i_len_count, (v_mk).m_U0.m_mk.m_outc, (v_mk).m_U0.m_mk.m_weightc)
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

		{
			(v_mk).m_U2.m_i = (i_len_count) * sizeof(uint32_t);

			__ae2fVK_AnnMlpMapRangedGeneric_imp(
					ae2f_float_t
					, (v_mk).m_U1.m_map
					, (*(v_mk).m_U0.m_mkswap.m_mkbase)
					, (v_mk).m_ret.m_err
					, (v_mk).m_U3.m_mapped
					, 0
					, (v_mk).m_U2.m_i
					);

			if((v_mk).m_ret.m_err) {
				assert(!"__ae2fVK_AnnMlpMapRanged_imp has failed.");
				__ae2fVK_AnnMlpUnMapRanged_imp(
						(v_mk).m_U1.m_unmap
						, *(v_mk).m_U0.m_mkswap.m_mkbase
						, 0, ((i_len_count) * sizeof(uint32_t))
						);
				break;
			}

			unless((v_mk).m_U3.m_mapped) {
				assert(!"__ae2fVK_AnnMlpMapRanged_imp went null");
				(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
			}

			(v_mk).m_U2.m_i = (i_len_count);
			while((v_mk).m_U2.m_i--) {
				ae2f_reinterpret_cast(uint32_t*, (v_mk).m_U3.m_mapped)[(v_mk).m_U2.m_i]
					= (i_len)[(v_mk).m_U2.m_i];
			}

			__ae2fVK_AnnMlpUnMapRanged_imp(
					(v_mk).m_U1.m_unmap
					, *(v_mk).m_U0.m_mkswap.m_mkbase
					, 0, ((i_len_count) * sizeof(uint32_t))
					);
		}

		/** Local memory allocation */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break
				, ((
						(v_mk).m_U0.m_mk.m_mkbase->m_outc * 
						((v_mk).m_U0.m_mk.m_mkbase->m_depth + 2)
				   ) * sizeof(cllocfloat_t))
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

		if((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres) {
			assert(!"__ae2fVK_AnnSlpMkAllocVKMem_imp");
			break;
		}

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
			(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		(v_mk).m_U1.m_vkpushconstrange.offset = 0;
		(v_mk).m_U1.m_vkpushconstrange.size = 0;
		(v_mk).m_U1.m_vkpushconstrange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		(v_mk).m_U2.m_vkpipelayoutcreatinfo.flags = 0;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pNext = NULL;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pPushConstantRanges 
			= &(v_mk).m_U1.m_vkpushconstrange;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pushConstantRangeCount = 1;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.setLayoutCount = 1;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.sType
			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pSetLayouts
			= (v_mk).m_U0.m_mkswap.m_mkbase->m_vkdescsetlayout;

		__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp(
				break
				, (v_mk).m_U1.m_vkpushconstrange
				, (v_mk).m_U2.m_vkpipelayoutcreatinfo
				, (v_mk).m_ret.m_err
				, (v_mk).m_U0.m_mkswap.m_mkbase
				, ae2fVK_eAnnMlpPipeLayouts_kPredict
				, sizeof(uint32_t) /* szrequired */
				, iv_vkalloccalls
				, i_vkdev
				);

		if((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp 0 has failed.");
			break;
		}

		__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp(
				break
				, (v_mk).m_U1.m_vkpushconstrange
				, (v_mk).m_U2.m_vkpipelayoutcreatinfo
				, (v_mk).m_ret.m_err
				, (v_mk).m_U0.m_mkswap.m_mkbase
				, ae2fVK_eAnnMlpPipeLayouts_kTrain
				, sizeof(uint32_t) + (sizeof(ae2f_float_t) * 2)
				, (iv_vkalloccalls)
				, (i_vkdev)
				);

		if((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp 0 has failed.");
			break;
		}

		assert(i_vkcldefinition);
		assert(i_vkcldeclaration);

		{
			__ae2fVK_AnnSlpMkCLSPVVerbose_imp(
					(v_mk).m_U1.m_clsrc_v
					, (v_mk).m_ret.m_err
					, (i_vkcldeclaration)
					, ae2fVK_AnnMlpSHADER
					, (i_vkcldefinition)
					);

			unless((v_mk).m_U1.m_clsrc_v) {
				assert(!"__ae2fVK_AnnSlpMkCLSPVVerbose_imp has failed.");
				(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
				break;
			}

			if(((v_mk).m_U4.m_clspverror = clspvCompileFromSourcesString(
							1, NULL
							, &(v_mk).m_U1.m_clsrc
							, ""
							, ae2f_const_cast(
								char**
								, ae2f_reinterpret_cast(
									const char**
									, &(v_mk).m_U2.m_vkshadermodcreatinfo.pCode
									)
								)
							, &(v_mk).m_U2.m_vkshadermodcreatinfo.codeSize
							, &(v_mk).m_U3.m_cllog
							)) != CLSPV_SUCCESS)
			{
				free((v_mk).m_U1.m_clsrc_v);
				free(ae2f_const_cast(uint32_t*, (v_mk).m_U2.m_vkshadermodcreatinfo.pCode));
				free((v_mk).m_U3.m_cllog);

				switch((v_mk).m_U4.m_clspverror) {
					case CLSPV_ERROR:
						(v_mk).m_ret.m_err |= ae2f_errGlob_NFOUND;
						assert(!"clspvCompileFromSourcesString has failed.");
						break;
					case CLSPV_OUT_OF_HOST_MEM:
						(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
						assert(!"clspvCompileFromSourcesString has failed its allocation.");
						break;

					case CLSPV_INVALID_ARG:
						(v_mk).m_ret.m_err |= ae2f_errGlob_WRONG_OPERATION;
						assert(!"clspvCompileFromSourcesString found arguments invalid.");
						break;

					case CLSPV_SUCCESS: abort();
				}

				break;
			}

			unless((v_mk).m_U2.m_vkshadermodcreatinfo.pCode) {
				assert(!"clspvCompileFromSourcesString went null.");
				(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
				break;
			}

			free((v_mk).m_U1.m_clsrc_v);

			(v_mk).m_U2.m_vkshadermodcreatinfo.sType 
				= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			(v_mk).m_U2.m_vkshadermodcreatinfo.flags = 0;
			(v_mk).m_U2.m_vkshadermodcreatinfo.pNext = NULL;

			if((((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres = vkCreateShaderModule(
								(i_vkdev)
								, &(v_mk).m_U2.m_vkshadermodcreatinfo
								, (iv_vkalloccalls)
								, &(v_mk).m_U0.m_mkswap.m_mkbase->m_vkshadermodule
								)) != VK_SUCCESS)) {
				assert(!"vkCreateShaderModule has failed.");
				free(ae2f_const_cast(uint32_t*, (v_mk).m_U2.m_vkshadermodcreatinfo.pCode));
				free((v_mk).m_U3.m_cllog);
				break;
			}

			unless((v_mk).m_U0.m_mkswap.m_mkbase->m_vkshadermodule) {
				assert(!"vkCreateShaderModule went null");
				free(ae2f_const_cast(uint32_t*, (v_mk).m_U2.m_vkshadermodcreatinfo.pCode));
				free((v_mk).m_U3.m_cllog);
				(v_mk).m_ret.m_err |= ae2f_errGlob_PTR_IS_NULL;
				break;
			}

			free(ae2f_const_cast(uint32_t*, (v_mk).m_U2.m_vkshadermodcreatinfo.pCode));
			free((v_mk).m_U3.m_cllog);
		}

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_ret.m_err
				, (v_mk).m_U1.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkpipelayout
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkshadermodule
				, "kPredict"
				, ae2fVK_eAnnMlpPipes_kPredict
				, ae2fVK_eAnnMlpPipeLayouts_kPredict
				);

		if((v_mk).m_ret.m_err) {
			assert(!"__ae2fVK_AnnSlpMkLoadPipeCreat_imp");
			break;
		}

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_ret.m_err
				, (v_mk).m_U1.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkpipelayout
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkshadermodule
				, "kPredictStream"
				, ae2fVK_eAnnMlpPipes_kPredictStream
				, ae2fVK_eAnnMlpPipeLayouts_kPredict
				);

		if((v_mk).m_ret.m_err) {
			assert(!"__ae2fVK_AnnSlpMkLoadPipeCreat_imp");
			break;
		}

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_ret.m_err
				, (v_mk).m_U1.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkpipelayout
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkshadermodule
				, "kFollow"
				, ae2fVK_eAnnMlpPipes_kFollow
				, ae2fVK_eAnnMlpPipeLayouts_kTrain
				);

		if((v_mk).m_ret.m_err) {
			assert(!"__ae2fVK_AnnSlpMkLoadPipeCreat_imp");
			break;
		}

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_ret.m_err
				, (v_mk).m_U1.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkpipelayout
				, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkshadermodule
				, "kTrainAuto"
				, ae2fVK_eAnnMlpPipes_kTrainAuto
				, ae2fVK_eAnnMlpPipeLayouts_kTrain
				);

		if((v_mk).m_ret.m_err) {
			assert(!"__ae2fVK_AnnSlpMkLoadPipeCreat_imp");
			break;
		}

		if(((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres 
					= vkCreateComputePipelines(
						(i_vkdev)
						, VK_NULL_HANDLE
						, ae2fVK_eAnnMlpPipes_LEN
						, (v_mk).m_U1.m_vkcomputepipecreatinfo
						, (iv_vkalloccalls)
						, (v_mk).m_U0.m_mkswap.m_mkbase->m_vkpipeline
						)
		   ) != VK_SUCCESS)
		{
			assert(!"vkCreateComputePipelines has failed.");
			break;
		}

		assert((v_mk).m_ret.m_err == ae2f_errGlob_OK);
		assert((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres == VK_SUCCESS);

		if((v_mk).m_ret.m_err) {
			assert(!"__ae2fVK_AnnSlpMkCreatDescPool has failed.");
			break;
		}
	} while(0);

	do {
		if(!(v_mk).m_U0.m_mkswap.m_mkbase) {
			assert(!"Allocation has failed.");
			(v_mk).m_ret.m_err |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		if((v_mk).m_U0.m_mkswap.m_mkbase->m_vkres != VK_SUCCESS) {
			assert(!"m_vkres was not VK_SUCCESS");
			(v_mk).m_ret.m_err |= ae2f_errGlob_NFOUND;
			break;
		}

		assert((v_mk).m_ret.m_err == ae2f_errGlob_OK && "Final status is no good");
	} while(0);
}

ae2f_MAC() _ae2fVK_AnnMlpClean_imp(ae2fVK_AnnMlp v_mlp) {
	vkDestroyBuffer(
			(v_mlp).m_vkdev
			, (v_mlp).m_vklocbuf
			, (v_mlp).m_vkalloccalls
		       );

	vkDestroyBuffer(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkglobbuf
			, (v_mlp).m_vkalloccalls
		       );

	vkDestroyPipeline(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkpipeline
			[ae2fVK_eAnnMlpPipes_kPredict]
			, (v_mlp).m_vkalloccalls
			);

	vkDestroyPipeline(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkpipeline
			[ae2fVK_eAnnMlpPipes_kFollow]
			, (v_mlp).m_vkalloccalls
			);

	vkDestroyPipeline(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkpipeline
			[ae2fVK_eAnnMlpPipes_kTrainAuto]
			, (v_mlp).m_vkalloccalls
			);

	vkDestroyPipeline(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkpipeline
			[ae2fVK_eAnnMlpPipes_kPredictStream]
			, (v_mlp).m_vkalloccalls
			);

	vkDestroyDescriptorSetLayout(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkdescsetlayout
			[ae2fVK_eAnnMlpDescLayouts_ONLY]
			, (v_mlp).m_vkalloccalls
			);

	vkFreeMemory(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkglobdevmem
			, (v_mlp).m_vkalloccalls
		    );
	vkFreeMemory(
			(v_mlp).m_vkdev
			, (v_mlp).m_vklocdevmem
			, (v_mlp).m_vkalloccalls
		    );

	vkDestroyPipelineLayout(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkpipelayout
			[ae2fVK_eAnnMlpPipeLayouts_kPredict]
			, (v_mlp).m_vkalloccalls
			);

	vkDestroyPipelineLayout(
			(v_mlp).m_vkdev
			, (v_mlp).m_vkpipelayout
			[ae2fVK_eAnnMlpPipeLayouts_kTrain]
			, (v_mlp).m_vkalloccalls
			);
}

ae2f_MAC() _ae2fVK_AnnMlpMapGoal_imp(
		ae2fVK_AnnMlpMapRangedGeneric_t	v_map,
		ae2fVK_AnnMlp			iv_mlp,
		ae2f_err_t			r_err,
		ae2f_float_t*			r_ptr
		)
{
	__ae2fVK_AnnSlpMapRangedGeneric_imp(
			ae2f_float_t
			, v_map
			, iv_mlp
			, r_err
			, r_ptr
			, __ae2fVK_AnnMlpGoalOff((iv_mlp).m_mlp.m_depth, (iv_mlp).m_mlp.m_outc, (iv_mlp).m_mlp.m_weightc)
			, __ae2fVK_AnnMlpGoalSz((iv_mlp).m_map.m_depth, (iv_mlp).m_mlp.m_outc, (iv_mlp).m_mlp.m_weightc) 
			);
}

ae2f_MAC() _ae2fVK_AnnMlpUnMapGoal_imp(
		ae2fVK_AnnMlpUnMap_t	v_unmap,
		ae2fVK_AnnMlp		iv_mlp
		)
{
	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_unmap
			, iv_mlp
			, __ae2fVK_AnnMlpGoalOff((iv_mlp).m_mlp.m_depth, (iv_mlp).m_mlp.m_outc, (iv_mlp).m_mlp.m_weightc)
			, __ae2fVK_AnnMlpGoalSz((iv_mlp).m_map.m_depth, (iv_mlp).m_mlp.m_outc, (iv_mlp).m_mlp.m_weightc) 
			);
}

ae2f_MAC() _ae2fVK_AnnMlpMapOutStream_imp(
		ae2fVK_AnnMlpMapRangedGeneric_t	v_map,
		ae2fVK_AnnMlp			iv_mlp,
		ae2f_err_t			r_err,
		ae2f_float_t*			r_ptr
		)
{
	__ae2fVK_AnnSlpMapRangedGeneric_imp(
			ae2f_float_t
			, v_map
			, iv_mlp
			, r_err
			, r_ptr
			, __ae2fVK_AnnMlpOutStreamOff((iv_mlp).m_mlp.m_depth, (iv_mlp).m_mlp.m_outc, (iv_mlp).m_mlp.m_weightc)
			, __ae2fVK_AnnMlpOutStreamSz((iv_mlp).m_mlp.m_depth, (iv_mlp).m_mlp.m_outc, (iv_mlp).m_mlp.m_weightc)
			);
}

ae2f_MAC() _ae2fVK_AnnMlpUnMapOutStream_imp(
		ae2fVK_AnnMlpUnMap_t	v_unmap,
		ae2fVK_AnnMlp		iv_mlp
		) 
{
	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_unmap
			, iv_mlp
			, sizeof(uint32_t) * (iv_mlp).m_mlp.m_depth
			, sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * (iv_mlp).m_mlp.m_depth
			);
}

ae2f_MAC() _ae2fVK_AnnMlpMapWB_imp(
		ae2fVK_AnnMlpMapRangedGeneric_t	v_map,
		ae2fVK_AnnMlp			iv_mlp,
		ae2f_err_t			r_err,
		ae2f_float_t*			r_ptrweight,
		ae2f_float_t*			r_ptrbias
		)
{
	__ae2fVK_AnnSlpMapRangedGeneric_imp(
			ae2f_float_t
			, v_map
			, iv_mlp
			, r_err
			, r_ptrweight
			, sizeof(uint32_t) * (iv_mlp).m_mlp.m_depth /** size vector */
			+ sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * (iv_mlp).m_mlp.m_depth /** ostream */
			, sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * ((iv_mlp).m_mlp.m_outc + 1) * ((iv_mlp).m_mlp.m_depth - 1)
			);
	do {
		if(r_err) {
			assert(!"__ae2fVK_AnnSlpMapRangedGeneric_imp has failed.");
			break;
		}

		unless(r_ptrweight) {
			assert(!"__ae2fVK_AnnSlpMapRangedGeneric_imp went null.");
			break;
		}

		r_ptrbias = 
			(r_ptrweight) + 
			((iv_mlp).m_mlp.m_depth - 1) * 
			(iv_mlp).m_mlp.m_outc * (iv_mlp).m_mlp.m_outc;

	} while(0);
}

ae2f_MAC() _ae2fVK_AnnMlpUnMapWB_imp(
		ae2fVK_AnnMlpUnMap_t	v_unmap,
		ae2fVK_AnnMlp		iv_mlp
		) 
{
	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_unmap
			, iv_mlp
			, sizeof(uint32_t) * (iv_mlp).m_mlp.m_depth
			+ sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * (iv_mlp).m_mlp.m_depth
			, sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * ((iv_mlp).m_mlp.m_outc + 1) * ((iv_mlp).m_mlp.m_depth - 1)
			);
}
#endif
