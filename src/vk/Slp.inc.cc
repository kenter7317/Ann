#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include "ae2f/Cast.h"
#include "ae2f/errGlob.h"
#include <vulkan/vulkan.h>
#include <ae2f/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include <ae2f/Pack/Beg.h>
#include <ae2f/Cmp.h>
#include <limits.h>

#define clspv_restrict restrict
#include <clspv/Compiler.h>

#if VK_MAX_MEMORY_TYPES >= UCHAR_MAX
#error "Sanity check: memory types will not be stored under unsigned char."
#endif

/** Each of them matches for the pipeline */
typedef enum ae2fVK_eAnnSlpPipes {
	ae2fVK_eAnnSlpPipes_kPredict = 0,
	ae2fVK_eAnnSlpPipes_kFollow,
	ae2fVK_eAnnSlpPipes_kTrain,
	ae2fVK_eAnnSlpPipes_kFit,

	/** @brief This is the end of the value, which typically means the count of actual kernel used. */
	ae2fVK_eAnnSlpPipes_LEN
} ae2fVK_eAnnSlpPipes;

typedef enum ae2fVK_eAnnSlpPipeLayouts {
	ae2fVK_eAnnSlpPipeLayouts_kPredict = 0,
	ae2fVK_eAnnSlpPipeLayouts_kFollow = 1,
	ae2fVK_eAnnSlpPipeLayouts_kTrain = 1,
	ae2fVK_eAnnSlpPipeLayouts_kFit = 2,
	ae2fVK_eAnnSlpPipeLayouts_LEN
} ae2fVK_eAnnSlpPipeLayouts;

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
	 * # Required constant resources
	 *
	 * Predict: \n	
	 * 	, size_t : InpSz			\n
	 * 	, ae2f_float_t[Inp]			\n
	 * 	, ae2f_float_t[Inp][Out] : Weight	\n
	 * 	, ae2f_float_t[Out] : Bias		\n
	 *
	 * Follow: \n
	 * 	ae2f_float_t[2] : Lr, LrBias	\n
	 * 	, ae2f_float_t[Inp]		\n
	 * 	, ae2f_float_t[Out] : Delta	\n
	 *
	 * Train: \n
	 * 	ae2f_float_t[2] : Lr, LrBias	\n
	 * 	, ae2f_float_t[Inp]		\n
	 * 	, ae2f_float_t[Out] : Goal
	 *
	 * Fit: \n
	 * 	ae2f_float_t[2] : Lr, LrBias	\n
	 * 	, ae2f_float_t[Inp]		\n
	 * 	, ae2f_float_t[Out] : Goal	\n
	 *	, ae2f_float_t[Out]
	 *
	 * */
	VkDescriptorSetLayout	m_vkdescsetlayout[1];
	VkPipelineLayout	m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_LEN];
	VkShaderModule		m_vkshadermodule;
	VkPipeline		m_vkpipeline[ae2fVK_eAnnSlpPipes_LEN];
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
	ClspvError				m_isgood;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkSPtr_t) {
	char*		m_char;
	void*		m_void;
	uint32_t*	m_wrds;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMk_t) {
	ae2fVK_AnnSlpMkUnion_t		m_union;
	ae2fVK_AnnSlpMkVKInfos_t	m_vkinfo;
	ae2fVK_AnnSlpMkVKStack_t	m_vkstack;

	const char* restrict		m_clsrc[3];
	size_t				m_clsrc_len[3];
	size_t				m_clout_len;
	char*				m_unused;
	ae2fVK_AnnSlpMkSPtr_t		m_clout;
	VkComputePipelineCreateInfo	m_pipecreat[4];

	/** @brief error */
	ae2f_err_t			m_reterr;
};

#include <ae2f/Pack/End.h>

#include <stdio.h> /** FIXME */

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

ae2f_MAC() _ae2fVK_AnnSlpMkLoadPipeCreat_imp(
		VkComputePipelineCreateInfo* const	pipecreat
		, VkPipelineLayout* const		pipelayout
		, const VkShaderModule			shadermod
		, const char* const			entrypoint
		, const ae2fVK_eAnnSlpPipes		idx_pipe
		, const ae2fVK_eAnnSlpPipeLayouts	idx_pipelayout
		)
{
	memset(
			&(pipecreat)[(idx_pipe)], 0
			, sizeof((pipecreat)[0])
	      );

	assert((pipelayout)[idx_pipelayout]);
	(pipecreat)[idx_pipe].sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	(pipecreat)[idx_pipe].layout = ((pipelayout)[idx_pipelayout]);
	(pipecreat)[idx_pipe].pNext = NULL;
	(pipecreat)[idx_pipe].basePipelineHandle = NULL;
	(pipecreat)[idx_pipe].basePipelineIndex = -1;
	(pipecreat)[idx_pipe].flags = 0;
	(pipecreat)[idx_pipe].stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	(pipecreat)[idx_pipe].stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	(pipecreat)[idx_pipe].stage.module = (shadermod);
	(pipecreat)[idx_pipe].stage.pName = entrypoint;
	(pipecreat)[idx_pipe].stage.pSpecializationInfo = NULL;
	(pipecreat)[idx_pipe].stage.flags = 0;
}

ae2f_MAC() _ae2fVK_AnnSlpMkOnePipeLayout_imp(
		ae2fVK_AnnSlpMk_t			v_mk
		, const ae2fVK_eAnnSlpPipeLayouts	idx
		, const size_t				szrequired
		, const VkAllocationCallbacks* const	vkalloccalls
		, const VkDevice			vkdev
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
 * Make slp with cl compute shader.
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
 * @param vkcldeclaration
 * @param vkcldefinition
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

		, const VkDevice					vkdev
		, const VkPhysicalDeviceMemoryProperties	vkmemprops
		, ae2f_opt VkAllocationCallbacks* const			vkalloccalls
		, ae2f_opt const VkBufferCreateInfo* const	vkbufinfo
		, ae2f_opt const VkMemoryAllocateInfo* const	vkmemallocinfo,

		const char* const					vkcldeclaration
		, const char* const					vkcldefinition
		)
{
	assert((vkdev) && "Vulkan device null check");
	(v_mk).m_reterr = ae2f_errGlob_OK;
	(v_mk).m_clout.m_void = NULL;

	do {
		__ae2f_AnnSlpMk_imp(
				(v_mk).m_union.m_base, (weight_opt)
				, (bias_opt), (cache_opt)
				, (inc), (outc)
				, ((prm_offset) + sizeof(ae2fVK_AnnSlp) - sizeof(ae2f_AnnSlp))
				, (extra)
				, (act), (actderiv), (lossderiv)
				, (learningrate), (learningrate_bias)
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
				, ae2fVK_eAnnSlpPipeLayouts_kPredict
				, (sizeof(size_t) + sizeof(ae2f_float_t) * ((inc) + (outc) + (inc) * (outc)))
				, vkalloccalls, vkdev
				);

		if ((v_mk).m_union.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 0 has failed.");
			break;
		}

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				v_mk
				, ae2fVK_eAnnSlpPipeLayouts_kFollow
				, sizeof(ae2f_float_t) * ((inc) + (outc) + 2)
				, vkalloccalls, vkdev
				);

		if ((v_mk).m_union.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 1 has failed.");
			break;
		}

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				v_mk
				, ae2fVK_eAnnSlpPipeLayouts_kFit
				, sizeof(ae2f_float_t) * ((inc) + (outc) * 2 + 2)
				, vkalloccalls, vkdev
				);

		if ((v_mk).m_union.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 2 has failed.");
			break;
		}

		assert((vkcldeclaration) && "vkcldeclaration is null");
		assert((vkcldefinition) && "vkcldefinition is null");

		(v_mk).m_clsrc[0] = (vkcldeclaration);
		(v_mk).m_clsrc[1] =
#include "cl/Slp.auto.cl.impl"
			;

		(v_mk).m_clsrc[2] = (vkcldefinition);
		(v_mk).m_unused = ae2f_static_cast(char*, NULL);
		(v_mk).m_clout_len = 0;
		(v_mk).m_clout.m_void = NULL;

		if(((v_mk).m_vkstack.m_isgood = clspvCompileFromSourcesString(
						3
						, ae2f_reinterpret_cast(const size_t*, NULL)
						, (v_mk).m_clsrc
						, "-Dae2fVK_clspv_IS_OPENCL_C=1"
						, &(v_mk).m_clout.m_char
						, &(v_mk).m_clout_len
						, &(v_mk).m_unused
						)) != CLSPV_SUCCESS)
		{
			assert(!"clspvCompileFromSourcesString has failed.");
			(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
			break;
		}

		assert((v_mk).m_union.m_base.m_ptr && "clspvCompileFromSourcesString has done something weird");

		(v_mk).m_vkinfo.m_shadermodulecreat.pNext = NULL;
		(v_mk).m_vkinfo.m_shadermodulecreat.codeSize
			= (v_mk).m_clout_len;

		(v_mk).m_vkinfo.m_shadermodulecreat.pCode
			= (v_mk).m_clout.m_wrds;

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
						, &(v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
						)) != VK_SUCCESS)
		{
			assert(!"vkCreateShaderModule has failed.");
			break;
		}

		unless((v_mk).m_union.m_alter.m_ptr->m_vkshadermodule) {
			assert(!"vkCreateShaderModule has produced NULL.");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				(v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kFollow"
				, (ae2fVK_eAnnSlpPipes_kFollow)
				, (ae2fVK_eAnnSlpPipeLayouts_kFollow)
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				(v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kFit"
				, (ae2fVK_eAnnSlpPipes_kFit)
				, (ae2fVK_eAnnSlpPipeLayouts_kFit)
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				(v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kPredict"
				, ae2fVK_eAnnSlpPipes_kPredict
				, ae2fVK_eAnnSlpPipeLayouts_kPredict
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				(v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kTrain"
				, (ae2fVK_eAnnSlpPipes_kTrain)
				, (ae2fVK_eAnnSlpPipeLayouts_kTrain)
				);

		if (((v_mk).m_union.m_alter.m_ptr->m_vkres
					= vkCreateComputePipelines(
						(vkdev)
						, VK_NULL_HANDLE
						, ae2fVK_eAnnSlpPipes_LEN
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

	clspvFreeOutputBuildObjs(
			(v_mk).m_clout.m_char
			, (v_mk).m_unused
			);

	assert((v_mk).m_reterr == ae2f_errGlob_OK && "Returned error flag has set up");
	if((v_mk).m_union.m_alter.m_ptr->m_vkres) {
		assert(!"Vulkan result check for validation");
		(v_mk).m_reterr |= ae2f_errGlob_NFOUND | ae2f_errGlob_ALLOC_FAILED;
	}
}

/** Map's usually for output */
ae2f_MAC() _ae2fVK_AnnSlpMap_imp(
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

ae2f_MAC() _ae2fVK_AnnSlpUnMap_imp(ae2fVK_AnnSlp slp) {
	assert((slp).m_vkdev);
	assert((slp).m_vkdevmem);
	vkUnmapMemory((slp).m_vkdev, (slp).m_vkdevmem);
}

ae2f_MAC() _ae2fVK_AnnSlpClean_imp(
		const ae2fVK_AnnSlp	block
		)
{
	assert((block).m_vkdev);

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

	if((block).m_vkshadermodule) {
		vkDestroyShaderModule(
				(block).m_vkdev
				, (block).m_vkshadermodule
				, (block).m_vkalloccalls
				);
	}
}

#endif
