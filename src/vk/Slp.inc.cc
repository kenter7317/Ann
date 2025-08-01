#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include "ae2f/Cast.h"
#include "ae2f/errGlob.h"
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Include/glslang_c_shader_types.h>
#include <glslang/Public/resource_limits_c.h>
#include <vulkan/vulkan.h>
#include <ae2f/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include <ae2f/Pack/Beg.h>
#include <ae2f/Cmp.h>

#include <ae2fVK/Spvc.h>

#include <limits.h>

#if VK_MAX_MEMORY_TYPES >= UCHAR_MAX
#error "Sanity check: memory types will not be stored under unsigned char."
#endif

typedef enum ae2fVK_eAnnSlpKernels {
	ae2fVK_eAnnSlpKernels_kPredict,	/** 0 */
	ae2fVK_eAnnSlpKernels_kFollow,	/** 1 */
	ae2fVK_eAnnSlpKernels_kTrain,	/** 1 */
	ae2fVK_eAnnSlpKernels_kFit,	/** 2 */

	/** This is the end of the value, which typically means the count of actual kernel used. */
	ae2fVK_eAnnSlpKernels_LEN
} ae2fVK_eAnnSlpKernels;

/**
 * @brief
 * Vulkan memory layout
 * [WeightBuffer] [BiasBuffer] [DeltaCacheBuffer]
 *
 * 
 * */
ae2f_structdef(struct, ae2fVK_AnnSlp)
{
	ae2f_AnnSlp	m_slp;

	VkDevice	m_vkdev;
	VkResult	m_vkres;
	VkBuffer	m_vkbuf;
	VkDeviceMemory	m_vkdevmem;
	VkAllocationCallbacks* restrict	m_vkalloccalls;

	/**
	 * @brief
	 * # Possible scenarioes
	 *
	 * Each contains constant pushes and memory layout.	\n
	 * Buffer for output.
	 *
	 * Output size is not important.	\n
	 * Input size(constant pushes) matters.
	 *
	 * 0: Predict	\n
	 * 1: Follow	(Same)	\n
	 * 2: Fit		\n
	 * 3: Train	(Same)
	 *
	 * # Required constant resources
	 *
	 * Predict:	
	 * 	sizeof(ae2f_float_t) * 2
	 * 	, sizeof(size_t) \n
	 * 	, sizeof(ae2f_float_t[Input])
	 *
	 * Follow:
	 * 	sizeof(ae2f_float_t) * 2 
	 * 	, sizeof(size_t)		\n
	 * 	, sizeof(ae2f_float_t[Input, Delta]) 
	 *
	 *
	 * Train:
	 * 	sizeof(ae2f_float_t) * 2	\n 
	 * 	, sizeof(size_t)	\n
	 * 	, sizeof(ae2f_float_t[Input, Goal]) 
	 *
	 * Fit:
	 * 	sizeof(ae2f_float_t) * 2  \n
	 * 	, sizeof(size_t)	\n
	 * 	, sizeof(ae2f_float_t[Input, Output, Goal]) 
	 *
	 * */
	VkDescriptorSetLayout	m_vkdescsetlayout[1];
	VkPipelineLayout	m_vkpipelayout[3];
	VkShaderModule		m_vkshadermodule[4];
	VkPipeline		m_vkpipeline[4];
};

ae2f_structdef(struct, ae2fVK_AnnSlpMkAlter_t) {
	/**
	 * @brief
	 * Stack size and allocation count.
	 */
	size_t m_stack, m_alloccount;
	/**
	 * @brief
	 * Pointer to the created SLP.
	 */
	ae2fVK_AnnSlp* restrict m_ptr;

	unsigned char		m_i;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkUnion_t) {
	ae2fVK_AnnSlpMkAlter_t	m_alter;
	ae2f_AnnSlpMk_t		m_base;
};

ae2f_structdef(union /*union*/, ae2fVK_AnnSlpMkVKInfos_t) {
	VkBufferCreateInfo		m_buf;
	VkMemoryAllocateInfo		m_alloc;
	VkPipelineLayoutCreateInfo	m_pipelayoutcreat;
	VkShaderModuleCreateInfo	m_shadermodulecreat;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkVK_t) {
	VkMemoryRequirements	m_req;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMkStackLayoutPredict_t) {
	VkDescriptorSetLayoutCreateInfo	m_creatinfo;
	VkDescriptorSetLayoutBinding	m_bind;
	VkPushConstantRange		m_pushconstant;
};

ae2f_structdef(union /*union*/, ae2fVK_AnnSlpMkVKStack_t) {
	VkMemoryRequirements			m_memreq;
	ae2fVK_AnnSlpMkStackLayoutPredict_t	m_layout;
	int					m_isgood;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkSPtr_t) {
	char*	m_char;
	void*	m_void;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMk_t) {
	ae2fVK_AnnSlpMkUnion_t		m_union;
	ae2fVK_AnnSlpMkVKInfos_t		m_vkinfo;
	ae2fVK_AnnSlpMkVKStack_t		m_vkstack;

	/** @brief error */
	ae2f_err_t			m_reterr;
	glslang_input_t			m_glslanginp;
	ae2fVK_AnnSlpMkSPtr_t		m_glslstdup;
	ae2fVK_Spvc			m_spvc;
	VkComputePipelineCreateInfo	m_pipecreat[4];
};

#include <ae2f/Pack/End.h>

#endif

#ifndef ae2fVK_Ann_Slp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2fVK/Ann/Slp.h>
#include <ae2f/Macro.h>
#endif

#define ae2fVK_Ann_Slp_c

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#endif

#define prm_Entry	""

ae2f_MAC(prm_Entry, ) _ae2fVK_AnnSlpMkOneShader_imp(
		ae2fVK_AnnSlpMk_t			v_mk
		, ae2fVK_Spvc				v_spvc
		, int					ret_isgood
		, const glslang_input_t* const		inp
		, char * const				Buffer
		, const VkDevice			vkdev
		, const VkAllocationCallbacks* const	vkalloccalls
		, const size_t				idx
		)
{
	do {
		memset(
				(Buffer) + sizeof("#version 450\n" "#define ") - 1
				, ' '
				, ae2f_CmpGetGt(
					sizeof("kPredict")
					, ae2f_CmpGetGt(
						sizeof("kTrain")
						, ae2f_CmpGetGt(
							sizeof("kFit")
							, sizeof("kFollow")
							)
						)
					)
		      );

		memcpy(
				(Buffer) + sizeof("#version 450\n" "#define ") - 1
				, prm_Entry
				, sizeof(prm_Entry) - 1
		      );

		__ae2fVK_SpvcMkWithMsg_imp(
				ret_isgood
				, v_spvc
				, inp
				, (
					GLSLANG_MSG_DEFAULT_BIT | 
					GLSLANG_MSG_SPV_RULES_BIT | 
					GLSLANG_MSG_VULKAN_RULES_BIT
				  )
				);

		/** Shader has made. */
		unless((ret_isgood)) {
			assert(!(prm_Entry " is considered as being the entry point."));
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		(v_mk).m_vkinfo.m_shadermodulecreat.pNext = NULL;
		(v_mk).m_vkinfo.m_shadermodulecreat.codeSize
			= (v_spvc).m_spirv_sz << 2;

		(v_mk).m_vkinfo.m_shadermodulecreat.pCode
			= (v_spvc).m_spirv_words;

		(v_mk).m_vkinfo.m_shadermodulecreat.pNext = NULL;
		(v_mk).m_vkinfo.m_shadermodulecreat.flags
			= 0;

		(v_mk).m_vkinfo.m_shadermodulecreat.pNext = NULL;
		(v_mk).m_vkinfo.m_shadermodulecreat.sType
			= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		assert((v_mk).m_vkinfo.m_shadermodulecreat.codeSize);
		assert((v_mk).m_vkinfo.m_shadermodulecreat.pCode);

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreateShaderModule(
						(vkdev)
						, &(v_mk).m_vkinfo.m_shadermodulecreat
						, (vkalloccalls)
						, (v_mk).m_union.m_alter.m_ptr
						->m_vkshadermodule + (idx)
						)) != VK_SUCCESS)
		{
			assert(!"vkCreateShaderModule has failed.");
			break;
		}

		/** Killing the shader object. */
		__ae2fVK_SpvcDel_imp((v_spvc));

		assert((v_mk).m_union.m_alter.m_ptr->m_vkshadermodule);

		memset(
				&(v_mk).m_pipecreat[(idx)], 0
				, sizeof((v_mk).m_pipecreat[0])
		      ); /** Strong reason */
		(v_mk).m_pipecreat[idx]
			.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		(v_mk).m_pipecreat[idx]
			.layout = (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout[
			(1 & (idx)) + (((idx) >> 1) & 1)
			];
		(v_mk).m_pipecreat[idx]
			.pNext = NULL;
		(v_mk).m_pipecreat[idx]
			.basePipelineHandle = NULL;
		(v_mk).m_pipecreat[idx]
			.basePipelineIndex = -1;
		(v_mk).m_pipecreat[idx]
			.flags = 0;

		assert((v_mk).m_union.m_alter.m_ptr->m_vkpipelayout[(1 & (idx)) + (((idx) >> 1) & 1)]);

		(v_mk).m_pipecreat[idx]
			.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		(v_mk).m_pipecreat[idx]
			.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		(v_mk).m_pipecreat[idx]
			.stage.module = (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule[idx];

		assert((v_mk).m_union.m_alter.m_ptr->m_vkshadermodule);

		(v_mk).m_pipecreat[idx]
			.stage.pName = "main";
		(v_mk).m_pipecreat[idx]
			.stage.pSpecializationInfo = NULL;
		(v_mk).m_pipecreat[idx]
			.stage.flags = 0;

	} while(0);
}

ae2f_MAC() _ae2fVK_AnnSlpMkOnePipeLayout_imp(
		ae2fVK_AnnSlpMk_t	v_mk
		, const size_t		idx
		, const size_t		szrequired
		, const VkAllocationCallbacks* const	vkalloccalls
		, const VkDevice	vkdev
		)
{
	(v_mk).m_vkstack.m_layout.m_pushconstant.size 
		= (szrequired);

	if((szrequired) & 0b11) {
		(v_mk).m_vkstack.m_layout.m_pushconstant.size 
			= ((szrequired) + 4)
			& ae2f_static_cast(uint32_t, ~0b11);
	}

	if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreatePipelineLayout(
					(vkdev)
					, &(v_mk).m_vkinfo.m_pipelayoutcreat
					, (vkalloccalls)
					, &(v_mk).m_union.m_alter.m_ptr->m_vkpipelayout[idx]
					)) != VK_SUCCESS)
	{
		assert(!"vkCreatePipelineLayout has failed.");
	}

	assert((v_mk).m_union.m_alter.m_ptr->m_vkpipelayout[idx]);

}

/**
 * @brief
 * Make slp with glsl compute shader.
 *
 * @details
 * The shader itself is incomplete. \n
 * For using it you will need to define three macros as function. \n
 * Below is the required snippet.
 *
 * #define ACT(const ae2f_float_t) -> ae2f_float_t \n
 * #define ACT_DERIV(const ae2f_float_t) -> ae2f_float_t \n
 * #define LOSS_DERIV(
 * 		const ae2f_float_t* const out
 * 		, const ae2f_float_t* const out_desired
 * 		, const size_t		index
 * 		, const size_t		length_out_goal
 * 		) -> ae2f_float_t
 *
 * @param vkglsldeclaration
 * @param vkglsldefinition
 * */
ae2f_MAC() _ae2fVK_AnnSlpMk_imp(
		ae2fVK_AnnSlpMk_t	v_mk
		, ae2f_float_t* const weight_opt
		, ae2f_float_t* const bias_opt
		, ae2f_float_t* const cache_opt
		, const uint32_t	inc
		, const uint32_t	outc
		, ae2f_opt const size_t	prm_offset
		, ae2f_opt const size_t	extra
		, ae2f_opt ae2f_AnnAct_t* const act
		, ae2f_opt ae2f_AnnAct_t* const actderiv
		, ae2f_AnnLoss_t* const lossderiv
		, ae2f_float_t learningrate
		, ae2f_float_t learningrate_bias

		, const VkDevice				vkdev
		, const VkPhysicalDeviceMemoryProperties	vkmemprops
		, ae2f_opt VkAllocationCallbacks* const		vkalloccalls
		, ae2f_opt const VkBufferCreateInfo* const	vkbufinfo
		, ae2f_opt const VkMemoryAllocateInfo* const	vkmemallocinfo,

		const char* const				vkglsldeclaration
		, const char* const				vkglsldefinition
		)
{
	assert((vkdev) && "Vulkan device null check");
	(v_mk).m_reterr = ae2f_errGlob_OK;
	(v_mk).m_glslstdup.m_void = NULL;

	do {
		__ae2f_AnnSlpMk_imp(
				(v_mk).m_union.m_base, weight_opt
				, bias_opt, cache_opt
				, inc, outc
				, (prm_offset) + sizeof(ae2fVK_AnnSlp) - sizeof(ae2f_AnnSlp)
				, extra
				, act, actderiv, lossderiv
				, learningrate, learningrate_bias
				);

		if((v_mk).m_union.m_base.m_ptr->m_Slp[0].m_inc > UINT32_MAX) {
			assert(!"Input size is too large");
			(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
			break;
		}

		if((v_mk).m_union.m_base.m_ptr->m_Slp[0].m_outc > UINT32_MAX) {
			assert(!"Output size is too large");
			(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
			break;
		}

		__ae2f_AnnSlpInitInpSz_imp((v_mk).m_union.m_base.m_alloccount, 0, 0, 0, inc, outc);
		(v_mk).m_union.m_base.m_alloccount 
			-= 	sizeof(ae2f_AnnSlp); /** weight, bias, deltacache */

		(v_mk).m_union.m_alter.m_ptr->m_vkdev = vkdev;
		(v_mk).m_union.m_alter.m_ptr->m_vkalloccalls = vkalloccalls;

		if(vkbufinfo) {
			(v_mk).m_vkinfo.m_buf = *(vkbufinfo);
			unless((vkbufinfo)->sType == VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO) {
				assert(!"vkbufinfo is not valid");
				(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
				break;
			}

			unless((vkbufinfo)->size >= (v_mk).m_union.m_base.m_alloccount) {
				assert(!"Least neeeded buffer size sanity check");
				(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
				break;
			}

			unless((vkbufinfo)->usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
				assert(!"Least usage bit check");
				(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
				break;
			}

			if(((v_mk).m_union.m_alter.m_ptr->m_vkres =
						vkCreateBuffer(
							vkdev
							, vkbufinfo
							, vkalloccalls
							, &(v_mk).m_union.m_alter.m_ptr->m_vkbuf
							)) != VK_SUCCESS) 
			{
				assert(!"Failed vkCreateBuffer");
				break;
			}
		} else {
			memset(&(v_mk).m_vkinfo.m_buf, 0, sizeof((v_mk).m_vkinfo.m_buf));
			(v_mk).m_vkinfo.m_buf
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			(v_mk).m_vkinfo.m_buf.size = (v_mk).m_union.m_base.m_alloccount;
			(v_mk).m_vkinfo.m_buf.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

			if(((v_mk).m_union.m_alter.m_ptr->m_vkres =
						vkCreateBuffer(
							vkdev
							, &(v_mk).m_vkinfo.m_buf
							, vkalloccalls
							, &(v_mk).m_union.m_alter.m_ptr->m_vkbuf
							)) != VK_SUCCESS) 
			{
				assert(!"Failed vkCreateBuffer");
				break;
			}
		}

		unless((v_mk).m_union.m_alter.m_ptr->m_vkbuf) {
			assert(!"m_vkbuf went null");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
		}


		vkGetBufferMemoryRequirements(
				(vkdev)
				, (v_mk).m_union.m_alter.m_ptr->m_vkbuf
				, &(v_mk).m_vkstack.m_memreq
				);


		for(
				(v_mk).m_union.m_alter.m_i = vkmemprops.memoryTypeCount;
				(v_mk).m_union.m_alter.m_i-- 
				&& (~(vkmemprops.memoryTypes[(v_mk).m_union.m_alter.m_i].propertyFlags)
					& VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				;
		   );


		if((v_mk).m_union.m_alter.m_i == UCHAR_MAX) {
			assert(!"VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is not supported");
			(v_mk).m_reterr |= ae2f_errGlob_IMP_NOT_FOUND;
			break;
		}

		assert((v_mk).m_vkstack.m_memreq.size >= (v_mk).m_vkinfo.m_buf.size);

		unless(
				(v_mk).m_vkstack.m_memreq.size 
				<= (vkmemprops).memoryHeaps
				[(vkmemprops).memoryTypes[(v_mk).m_union.m_alter.m_i].heapIndex]
				.size
		      )
		{
			assert(!"size is too big");
			(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
			break;
		}

		if(vkmemallocinfo) {
			(v_mk).m_vkinfo.m_alloc = *(vkmemallocinfo);
			if((vkmemallocinfo)->allocationSize < (v_mk).m_vkstack.m_memreq.size) {
				assert(!"allocation size is not enough");
				(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
				break;
			}

			unless((vkmemprops).memoryTypes[(vkmemallocinfo)->memoryTypeIndex].propertyFlags 
					& VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				assert(!"Memory type does not contain VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
				(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION | ae2f_errGlob_IMP_NOT_FOUND;
				break;
			}

		} else {
			(v_mk).m_vkinfo.m_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			(v_mk).m_vkinfo.m_alloc.allocationSize = (v_mk).m_vkstack.m_memreq.size;
			(v_mk).m_vkinfo.m_alloc.memoryTypeIndex = (v_mk).m_union.m_alter.m_i;
			(v_mk).m_vkinfo.m_alloc.pNext = 0;
		}

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = 
					vkAllocateMemory(
						(vkdev)
						, &(v_mk).m_vkinfo.m_alloc
						, (v_mk).m_union.m_alter.m_ptr->m_vkalloccalls
						, &(v_mk).m_union.m_alter.m_ptr->m_vkdevmem
						)) != VK_SUCCESS)
		{
			assert(!"Failed vkAllocateMemory");
			break;
		}

		unless((v_mk).m_union.m_alter.m_ptr->m_vkdevmem) 
		{
			assert(!"m_vkdevmem went null");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = 
					vkBindBufferMemory(
						(vkdev)
						, (v_mk).m_union.m_alter.m_ptr->m_vkbuf
						, (v_mk).m_union.m_alter.m_ptr->m_vkdevmem
						, 0
						)) != VK_SUCCESS)
		{
			assert(!"Failed vkBindBufferMemory");
			break;
		}

		(v_mk).m_vkstack.m_layout.m_bind.binding = 0;
		(v_mk).m_vkstack.m_layout.m_bind.descriptorType
			= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		(v_mk).m_vkstack.m_layout.m_bind.descriptorCount = 1;
		(v_mk).m_vkstack.m_layout.m_bind.stageFlags
			= VK_SHADER_STAGE_COMPUTE_BIT;
		(v_mk).m_vkstack.m_layout.m_bind.pImmutableSamplers = NULL;

		(v_mk).m_vkstack.m_layout.m_creatinfo.pBindings
			= &(v_mk).m_vkstack.m_layout.m_bind;
		(v_mk).m_vkstack.m_layout.m_creatinfo.bindingCount = 1;
		(v_mk).m_vkstack.m_layout.m_creatinfo.flags
			= 0;
		(v_mk).m_vkstack.m_layout.m_creatinfo.pNext = NULL;
		(v_mk).m_vkstack.m_layout.m_creatinfo.sType
			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreateDescriptorSetLayout(
						(vkdev)
						, &(v_mk).m_vkstack.m_layout.m_creatinfo
						, (vkalloccalls)
						, (v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout
						)) != VK_SUCCESS) {
			assert(!"vkCreateDescriptorSetLayout has failed");
			break;
		}

		memset(
				&(v_mk).m_vkinfo.m_pipelayoutcreat
				, 0, sizeof((v_mk).m_vkinfo.m_pipelayoutcreat)
		      );

		(v_mk).m_vkstack.m_layout.m_pushconstant.offset = 0;
		(v_mk).m_vkstack.m_layout.m_pushconstant.stageFlags
			= VK_SHADER_STAGE_COMPUTE_BIT;

		(v_mk).m_vkinfo.m_pipelayoutcreat.sType 
			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		(v_mk).m_vkinfo.m_pipelayoutcreat.setLayoutCount = 1;
		(v_mk).m_vkinfo.m_pipelayoutcreat.pushConstantRangeCount = 1;

		(v_mk).m_vkinfo.m_pipelayoutcreat.pPushConstantRanges
			= &(v_mk).m_vkstack.m_layout.m_pushconstant;



		(v_mk).m_vkinfo.m_pipelayoutcreat.pSetLayouts
			= (v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout;

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				v_mk
				, 0
				, sizeof(size_t) + (sizeof(ae2f_float_t) * 2)
				+ sizeof(ae2f_float_t) * ((inc))
				, vkalloccalls, vkdev
				);

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				v_mk
				, 1
				, sizeof(size_t) + (sizeof(ae2f_float_t) * 2)
				+ sizeof(ae2f_float_t) * ((inc) + (outc))
				, vkalloccalls, vkdev
				);

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				v_mk
				, 2
				, sizeof(size_t) + (sizeof(ae2f_float_t) * 2)
				+ sizeof(ae2f_float_t) * ((inc) + ((outc) * 2))
				, vkalloccalls, vkdev
				);


		assert((vkglsldeclaration) && "vkglsldeclaration is null");
		assert((vkglsldefinition) && "vkglsldefinition is null");

		(v_mk).m_glslstdup.m_void =  
			calloc(sizeof(
						"#version 450\n"
#include "./glsl/Slp.auto.glsl.impl"
					    )
					+ strlen(vkglsldeclaration)
					+ strlen(vkglsldefinition)
					+ 1
					+ ae2f_CmpGetGt(
						sizeof("kPredict")
						, ae2f_CmpGetGt(
							sizeof("kTrain")
							, ae2f_CmpGetGt(
								sizeof("kFit")
								, sizeof("kFollow")
								)
							)
						) - 1
					+ sizeof("#define ") - 1 
					+ sizeof(" main\n")
					, 1);

		unless((v_mk).m_glslstdup.m_void) {
			assert(!"Failed allocating");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		strcpy(
				(v_mk).m_glslstdup.m_char
				,"#version 450\n" "#define "	
		      );

		memset(
				(v_mk).m_glslstdup.m_char + sizeof("#version 450\n" "#define ") - 1
				, ' '
				, ae2f_CmpGetGt(
					sizeof("kPredict")
					, ae2f_CmpGetGt(
						sizeof("kTrain")
						, ae2f_CmpGetGt(
							sizeof("kFit")
							, sizeof("kFollow")
							)
						)
					)
		      );

		strcat((v_mk).m_glslstdup.m_char, "main\n");

		strcat(
				(v_mk).m_glslstdup.m_char
				, vkglsldeclaration
		      );

		strcat(
				(v_mk).m_glslstdup.m_char
				, "\n"
		      );

		strcat(
				(v_mk).m_glslstdup.m_char,
#include "./glsl/Slp.auto.glsl.impl"
		      );

		strcat(
				(v_mk).m_glslstdup.m_char
				, vkglsldefinition
		      );

		(v_mk).m_glslanginp.language = GLSLANG_SOURCE_GLSL;
		(v_mk).m_glslanginp.stage =  GLSLANG_STAGE_COMPUTE;
		(v_mk).m_glslanginp.client = GLSLANG_CLIENT_VULKAN;
		(v_mk).m_glslanginp.client_version = GLSLANG_TARGET_VULKAN_1_4;
		(v_mk).m_glslanginp.target_language = GLSLANG_TARGET_SPV;
		(v_mk).m_glslanginp.target_language_version = GLSLANG_TARGET_SPV_1_6;
		(v_mk).m_glslanginp.default_version = 450;
		(v_mk).m_glslanginp.default_profile = GLSLANG_NO_PROFILE;
		(v_mk).m_glslanginp.force_default_version_and_profile = 0;
		(v_mk).m_glslanginp.forward_compatible = 0;
		(v_mk).m_glslanginp.messages = GLSLANG_MSG_DEFAULT_BIT;
		(v_mk).m_glslanginp.code = (v_mk).m_glslstdup.m_char;

		(v_mk).m_glslanginp.callbacks.free_include_result = 0;
		(v_mk).m_glslanginp.callbacks.include_local = 0;
		(v_mk).m_glslanginp.callbacks.include_system = 0;
		(v_mk).m_glslanginp.callbacks_ctx = NULL;

		(v_mk).m_glslanginp.resource = glslang_default_resource();



		__ae2fVK_AnnSlpMkOneShader_imp(
				"kPredict"
				, (v_mk)
				, (v_mk).m_spvc
				, (v_mk).m_vkstack.m_isgood
				, &(v_mk).m_glslanginp
				, (v_mk).m_glslstdup.m_char
				, vkdev
				, vkalloccalls
				, ae2fVK_eAnnSlpKernels_kPredict
				);

		__ae2fVK_AnnSlpMkOneShader_imp(
				"kFollow"
				, (v_mk)
				, (v_mk).m_spvc
				, (v_mk).m_vkstack.m_isgood
				, &(v_mk).m_glslanginp
				, (v_mk).m_glslstdup.m_char
				, vkdev
				, vkalloccalls
				, ae2fVK_eAnnSlpKernels_kFollow
				);

		__ae2fVK_AnnSlpMkOneShader_imp(
				"kFit"
				, (v_mk)
				, (v_mk).m_spvc
				, (v_mk).m_vkstack.m_isgood
				, &(v_mk).m_glslanginp
				, (v_mk).m_glslstdup.m_char
				, vkdev, vkalloccalls
				, ae2fVK_eAnnSlpKernels_kFit
				);

		__ae2fVK_AnnSlpMkOneShader_imp(
				"kTrain"
				, (v_mk)
				, (v_mk).m_spvc
				, (v_mk).m_vkstack.m_isgood
				, &(v_mk).m_glslanginp
				, (v_mk).m_glslstdup.m_char
				, vkdev, vkalloccalls
				, ae2fVK_eAnnSlpKernels_kTrain
				);

		if (((v_mk).m_union.m_alter.m_ptr->m_vkres
					= vkCreateComputePipelines(
						(vkdev)
						, VK_NULL_HANDLE
						, ae2fVK_eAnnSlpKernels_LEN
						, (v_mk).m_pipecreat
						, (vkalloccalls)
						, (v_mk).m_union.m_alter.m_ptr->m_vkpipeline
						)
		    ) != VK_SUCCESS)
		{
			assert(!"vkCreateComputePipelines has failed.");
			break;
		}
	} while(0);

	if((v_mk).m_glslstdup.m_void) {
		free((v_mk).m_glslstdup.m_void);
	}

	assert((v_mk).m_reterr == ae2f_errGlob_OK && "Returned error flag has set up");
	if((v_mk).m_union.m_alter.m_ptr->m_vkres) {
		assert(!"Vulkan result check for validation");
		(v_mk).m_reterr |= ae2f_errGlob_NFOUND | ae2f_errGlob_ALLOC_FAILED;
	}
}

/** Map's usually for output */
ae2f_MAC() _ae2fVK_AnnSlpMap(
		ae2fVK_AnnSlp	slp
		, ae2f_float_t** restrict const data
		)
{
	if((slp).m_vkres != VK_SUCCESS) {
		assert(!"Result flag is not valid.");
	}

	assert((slp).m_vkdev);
	assert((slp).m_vkdevmem);

	(slp).m_vkres = vkMapMemory(
			(slp).m_vkdev
			, (slp).m_vkdevmem
			, 0, VK_WHOLE_SIZE
			, 0
			, ae2f_reinterpret_cast(void** restrict, data)
			);

	assert((slp).m_vkres == VK_SUCCESS && "vkMapMemory has failed.");
	assert(*(data) && "Mapped as null");
}

ae2f_MAC() _ae2fVK_AnnSlpUnMap(ae2fVK_AnnSlp slp) {
	assert((slp).m_vkdev);
	assert((slp).m_vkdevmem);
	vkUnmapMemory((slp).m_vkdev, (slp).m_vkdevmem);
}

ae2f_MAC() _ae2fVK_AnnSlpClean(
		const ae2fVK_AnnSlp	block
		)
{
	if((block).m_vkbuf) {
		vkDestroyBuffer(
				(block).m_vkdev
				, (block).m_vkbuf
				, (block).m_vkalloccalls
			       );
	}


	if((block).m_vkdevmem) {
		vkFreeMemory(
				(block).m_vkdev
				, (block).m_vkdevmem
				, (block).m_vkalloccalls
			    );
	}

	if((block).m_vkdescsetlayout[0]) {
		vkDestroyDescriptorSetLayout(
				(block).m_vkdev
				, (block).m_vkdescsetlayout[0]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipelayout[0]) {
		vkDestroyPipelineLayout(
				(block).m_vkdev
				, (block).m_vkpipelayout[0]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipelayout[1]) {
		vkDestroyPipelineLayout(
				(block).m_vkdev
				, (block).m_vkpipelayout[1]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipelayout[2]) {
		vkDestroyPipelineLayout(
				(block).m_vkdev
				, (block).m_vkpipelayout[2]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkshadermodule[0]) {
		vkDestroyShaderModule(
				(block).m_vkdev
				, (block).m_vkshadermodule[0]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkshadermodule[1]) {
		vkDestroyShaderModule(
				(block).m_vkdev
				, (block).m_vkshadermodule[1]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkshadermodule[2]) {
		vkDestroyShaderModule(
				(block).m_vkdev
				, (block).m_vkshadermodule[2]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkshadermodule[3]) {
		vkDestroyShaderModule(
				(block).m_vkdev
				, (block).m_vkshadermodule[3]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipeline[0]) {
		vkDestroyPipeline(
				(block).m_vkdev
				, (block).m_vkpipeline[0]
				, (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipeline[1]) {
		vkDestroyPipeline(
				(block).m_vkdev
				, (block).m_vkpipeline[1]
				,  (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipeline[2]) {
		vkDestroyPipeline(
				(block).m_vkdev
				, (block).m_vkpipeline[2]
				,  (block).m_vkalloccalls
				);
	}

	if((block).m_vkpipeline[3]) {
		vkDestroyPipeline(
				(block).m_vkdev
				, (block).m_vkpipeline[3]
				,  (block).m_vkalloccalls
				);
	}
}

#endif
