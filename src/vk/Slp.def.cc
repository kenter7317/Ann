#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include "ae2f/Float.auto.h"
#include "ae2f/errGlob.h"
#include <vulkan/vulkan.h>
#include <ae2f/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include <ae2f/Pack/Beg.h>

#include <limits.h>

#if VK_MAX_MEMORY_TYPES >= UCHAR_MAX
#error "Sanity check: memory types will not be stored under unsigned char."
#endif

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
	 * 2: Fit	(Same)	\n
	 * 3: Train	(Same)
	 *
	 * # Required constant resources
	 *
	 * Predict:	
	 * 	sizeof(ae2f_float_t[Input, Weights, Bias]) + sizeof(size_t)	\n
	 *
	 * Follow:
	 * 	sizeof(ae2f_float_t[Input, Weights, Bias, Delta]) + sizeof(size_t)	\n
	 *
	 * Fit:
	 * 	sizeof(ae2f_float_t[Input, Weights, Bias, Output]) + sizeof(size_t)	\n
	 *
	 * Train:
	 * 	sizeof(ae2f_float_t[Input, Weights, Bias, Output]) + sizeof(size_t)	\n
	 *
	 * */
	VkDescriptorSetLayout	m_vkdescsetlayout[1];
	VkPipelineLayout	m_vkpipelayout[2];
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

ae2f_structdef(union, ae2fVK_AnnSlpMkVKInfos_t) {
	char			m_init;
	VkBufferCreateInfo		m_buf;
	VkMemoryAllocateInfo		m_alloc;
	VkPipelineLayoutCreateInfo	m_pipelayoutcreat;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkVK_t) {
	char			m_init;
	VkMemoryRequirements	m_req;	
};

ae2f_structdef(struct, ae2fVK_AnnSlpMkStackLayoutPredict_t) {
	VkDescriptorSetLayoutCreateInfo	m_creatinfo;
	VkDescriptorSetLayoutBinding	m_bind;
	VkPushConstantRange		m_pushconstant;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkVKStack_t) {
	VkMemoryRequirements			m_memreq;
	ae2fVK_AnnSlpMkStackLayoutPredict_t	m_layout;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMk_t) {
	ae2fVK_AnnSlpMkUnion_t		m_union;
	ae2fVK_AnnSlpMkVKInfos_t	m_vkinfo;
	ae2fVK_AnnSlpMkVKStack_t	m_vkstack;

	/** @brief error */
	ae2f_err_t			m_reterr;
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

#endif


ae2f_MAC() _ae2fVK_AnnSlpMk_imp(
		ae2fVK_AnnSlpMk_t	v_mk
		, ae2f_float_t* const weight_opt
		, ae2f_float_t* const bias_opt
		, ae2f_float_t* const cache_opt
		, const size_t		inc
		, const size_t		outc
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
		, ae2f_opt const VkMemoryAllocateInfo* const	vkmemallocinfo
		)
{
	assert((vkdev) && "Vulkan device null check");
	(v_mk).m_reterr = ae2f_errGlob_OK;

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

		__ae2f_AnnSlpInitInpSz_imp((v_mk).m_union.m_base.m_alloccount, 0, 0, 0, inc, outc);
		(v_mk).m_union.m_base.m_alloccount 
			-=	sizeof(ae2f_AnnSlp);

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
			(v_mk).m_vkinfo.m_init = 0;
			(v_mk).m_vkinfo.m_buf.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
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

		if(vkmemallocinfo) {
			(v_mk).m_vkinfo.m_alloc = *(vkmemallocinfo);
			if((vkmemallocinfo)->allocationSize < (v_mk).m_vkstack.m_memreq.size) {
				assert(!"allocation size is not enough");
				(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
				break;
			}

			if((vkmemprops).memoryTypes[(vkmemallocinfo)->memoryTypeIndex].propertyFlags 
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

		(v_mk).m_vkstack.m_layout.m_pushconstant.size 
			= 
			sizeof(size_t) 
			+ sizeof(ae2f_float_t) * (
					(v_mk).m_union.m_alter.m_alloccount - (outc) + (inc)
					);

		if((v_mk).m_vkstack.m_layout.m_pushconstant.size & 0b11) {
			(v_mk).m_vkstack.m_layout.m_pushconstant.size 
				= ((v_mk).m_vkstack.m_layout.m_pushconstant.size + 4)
				& ae2f_static_cast(uint32_t, ~0b11);
		}

		(v_mk).m_vkstack.m_layout.m_pushconstant.offset = 0;
		(v_mk).m_vkstack.m_layout.m_pushconstant.stageFlags
			= VK_SHADER_STAGE_COMPUTE_BIT;

		(v_mk).m_vkstack.m_layout.m_bind.binding = 1;
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
			= VK_SHADER_STAGE_COMPUTE_BIT;
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

		(v_mk).m_vkinfo.m_init = 0;
		(v_mk).m_vkinfo.m_pipelayoutcreat.sType 
			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		(v_mk).m_vkinfo.m_pipelayoutcreat.setLayoutCount = 1;
		(v_mk).m_vkinfo.m_pipelayoutcreat.pushConstantRangeCount = 1;

		(v_mk).m_vkinfo.m_pipelayoutcreat.pPushConstantRanges
			= &(v_mk).m_vkstack.m_layout.m_pushconstant;
		(v_mk).m_vkinfo.m_pipelayoutcreat.pSetLayouts
			= (v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout;

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreatePipelineLayout(
						(vkdev)
						, &(v_mk).m_vkinfo.m_pipelayoutcreat
						, (vkalloccalls)
						, (v_mk).m_union.m_alter
						.m_ptr->m_vkpipelayout
						)) != VK_SUCCESS) 
		{
			assert(!"vkCreatePipelineLayout for Predict-like has failed.");
			break;
		}

		(v_mk).m_vkstack.m_layout.m_pushconstant.size 
			= 
			sizeof(size_t) 
			+ sizeof(ae2f_float_t) * (
					(v_mk).m_union.m_alter.m_alloccount + (inc)
					);

		if((v_mk).m_vkstack.m_layout.m_pushconstant.size & 0b11) {
			(v_mk).m_vkstack.m_layout.m_pushconstant.size 
				= ((v_mk).m_vkstack.m_layout.m_pushconstant.size + 4)
				& ae2f_static_cast(uint32_t, ~0b11);
		}

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreatePipelineLayout(
						(vkdev)
						, &(v_mk).m_vkinfo.m_pipelayoutcreat
						, (vkalloccalls)
						, (v_mk).m_union.m_alter
						.m_ptr->m_vkpipelayout + 1
						)) != VK_SUCCESS) 
		{
			assert(!"vkCreatePipelineLayout for Predict-like has failed.");
			break;
		}
	} while(0);

	assert((v_mk).m_reterr == ae2f_errGlob_OK && "Returned error flag has set up");
	if((v_mk).m_union.m_alter.m_ptr->m_vkres) {
		assert(!"Vulkan result check for validation");
		(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
	}
}

/** Map's usually for output */
ae2f_MAC() _ae2fVK_AnnSlpMap(
		ae2fVK_AnnSlp	slp
		, ae2f_float_t** restrict const data
		)
{
	if((slp).m_vkres != VK_SUCCESS) {
		assert(0 && "Result flag is not valid.");
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
		ae2fVK_AnnSlp	block
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
}

#endif
