#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include <vulkan/vulkan.h>
#include <ae2f/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include <ae2f/Pack/Beg.h>

#include <limits.h>

#if VK_MAX_MEMORY_TYPES >= UCHAR_MAX
#error "Sanity check: memory types will not be stored under unsigned char."
#endif

ae2f_structdef(struct, ae2fVK_AnnSlp)
{
	ae2f_AnnSlp	m_slp;

	VkDevice	m_vkdev;
	VkResult	m_vkres;
	VkBuffer	m_vkbuf;
	VkDeviceMemory	m_vkdevmem;

	VkAllocationCallbacks*	m_vkalloccalls;
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
	VkBufferCreateInfo	m_buf;
	VkMemoryAllocateInfo	m_alloc;
	char			m_init;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMk_t) {
	ae2fVK_AnnSlpMkUnion_t		m_union;
	ae2fVK_AnnSlpMkVKInfos_t	m_vkinfo;
	VkMemoryRequirements		m_vkmemreq;
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
		, ae2f_opt const size_t	offset
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

	do {
		__ae2f_AnnSlpMk_imp(
				(v_mk).m_union.m_base, weight_opt
				, bias_opt, cache_opt
				, inc, outc
				, (offset) + sizeof(ae2fVK_AnnSlp) - sizeof(ae2f_AnnSlp)
				, extra
				, act, actderiv, lossderiv
				, learningrate, learningrate_bias
				);

		__ae2f_AnnSlpInitInpSz_imp((v_mk).m_union.m_base.m_alloccount, 0, 0, 0, inc, outc);
		(v_mk).m_union.m_base.m_alloccount 
			+=	sizeof(ae2f_float_t) * ((outc))
			-	sizeof(ae2f_AnnSlp);

		(v_mk).m_union.m_alter.m_ptr->m_vkdev = vkdev;
		(v_mk).m_union.m_alter.m_ptr->m_vkalloccalls = vkalloccalls;

		if(vkbufinfo) {
			(v_mk).m_vkinfo.m_buf = *(vkbufinfo);
			assert((v_mk).m_vkinfo.m_buf.sType == VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);
		} else {
			(v_mk).m_vkinfo.m_init = 0;
			(v_mk).m_vkinfo.m_buf.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			(v_mk).m_vkinfo.m_buf.size = (v_mk).m_union.m_base.m_alloccount;
			assert((v_mk).m_vkinfo.m_buf.size && "Alloccount is 0");

			(v_mk).m_vkinfo.m_buf.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		assert((v_mk).m_vkinfo.m_buf.sType == VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);

		assert((v_mk).m_vkinfo.m_buf.size >= (v_mk).m_union.m_base.m_alloccount 
				&& "Least neeeded buffer size sanity check"
		      );

		assert((v_mk).m_vkinfo.m_buf.usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
				&& "Least usage bit check"
		      );

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres =
					vkCreateBuffer(
						vkdev
						, &(v_mk).m_vkinfo.m_buf
						, vkalloccalls
						, &(v_mk).m_union.m_alter.m_ptr->m_vkbuf
						)) != VK_SUCCESS) break;

		assert((v_mk).m_union.m_alter.m_ptr->m_vkres == VK_SUCCESS && 
				"Failed vkCreateBuffer");

		assert((v_mk).m_union.m_alter.m_ptr->m_vkbuf && "m_vkbuf went null");

		vkGetBufferMemoryRequirements(
				(vkdev)
				, (v_mk).m_union.m_alter.m_ptr->m_vkbuf
				, &(v_mk).m_vkmemreq
				);

		assert((v_mk).m_vkmemreq.size && "size is 0 on line 161"); /** ? */
		assert((v_mk).m_vkmemreq.size >= (v_mk).m_vkinfo.m_buf.size);
		assert(
				(v_mk).m_vkmemreq.size 
				<= (vkmemprops).memoryHeaps
				[(vkmemprops).memoryTypes[(v_mk).m_union.m_alter.m_i].heapIndex]
				.size && "Size is too big"
				);

		for(
				(v_mk).m_union.m_alter.m_i = vkmemprops.memoryTypeCount;
				(v_mk).m_union.m_alter.m_i-- 
				&& (~(vkmemprops.memoryTypes[(v_mk).m_union.m_alter.m_i].propertyFlags)
					& VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				;
		   );

		if(
				(v_mk).m_union.m_alter.m_i == UCHAR_MAX 
				|| (v_mk).m_union.m_alter.m_i == (unsigned char)-1) 
		{
			assert(0 && "VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is not supported");
			break;
		}

		if(vkmemallocinfo) {
			(v_mk).m_vkinfo.m_alloc = *(vkmemallocinfo);
		} else {
			(v_mk).m_vkinfo.m_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			(v_mk).m_vkinfo.m_alloc.allocationSize = 0;
			(v_mk).m_vkinfo.m_alloc.memoryTypeIndex = (v_mk).m_union.m_alter.m_i;
			(v_mk).m_vkinfo.m_alloc.pNext = 0;
		}

		(v_mk).m_vkinfo.m_alloc.allocationSize += (v_mk).m_vkmemreq.size;

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = 
					vkAllocateMemory(
						(vkdev)
						, &(v_mk).m_vkinfo.m_alloc
						, (v_mk).m_union.m_alter.m_ptr->m_vkalloccalls
						, &(v_mk).m_union.m_alter.m_ptr->m_vkdevmem
						)) != VK_SUCCESS)
		{
			assert(0 && "Failed vkAllocateMemory");
			break;
		}
		assert((v_mk).m_union.m_alter.m_ptr->m_vkdevmem &&
				"m_vkdevmem went null"
		      );
	} while(0);

	assert((v_mk).m_union.m_alter.m_ptr->m_vkres == VK_SUCCESS && 
			"Vulkan result check for validation"
	      );
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
}

#endif
