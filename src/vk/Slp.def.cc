#ifndef ae2fVK_Ann_Slp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#endif

#include <ae2fVK/Ann/Slp.h>

#define ae2fVK_Ann_Slp_c

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define CMDONERR
#endif

#include <ae2fVK/Ann/SlpSHADER.auto.h>

ae2f_MAC(CMDONERR, ) _ae2fVK_AnnSlpMkFndMemProp_imp(
		unsigned char				r_memtypeidx,
		ae2f_err_t				v_errbit,
		const unsigned char			memtypeidx_invalid_minus_one,
		const VkMemoryPropertyFlagBits		vkpropflag_req,
		const VkPhysicalDeviceMemoryProperties	vkphydevmemprops
		)
{

	assert(VK_MAX_MEMORY_TYPES <= memtypeidx_invalid_minus_one && "Invalid index does not do its thing.");
	assert((vkphydevmemprops).memoryTypeCount <= VK_MAX_MEMORY_TYPES && "Invalid memory type count.");

	for(
			(r_memtypeidx) = (vkphydevmemprops).memoryTypeCount;
			(r_memtypeidx)-- 
			&& (~((vkphydevmemprops).memoryTypes[(r_memtypeidx)].propertyFlags)
				& vkpropflag_req)
			;
	   );

	if((r_memtypeidx) == (memtypeidx_invalid_minus_one)) {
		assert(!"Wanted bit has not found is not supported");
		(v_errbit) |= ae2f_errGlob_IMP_NOT_FOUND;
		CMDONERR;
	}
}

ae2f_MAC(CMDONERR, ) _ae2fVK_AnnSlpMkCreatBuf(
		const size_t			sz,
		VkResult			r_vkres,
		VkBuffer			r_vkbuf,

		VkBufferCreateInfo		v_vkbufcreatinfo,
		ae2f_err_t			v_reterr,

		const VkBufferUsageFlags	vkbufuseflags,
		const VkSharingMode		vksharemode,
		const VkDevice			vkdev,
		ae2f_opt const VkAllocationCallbacks*	vkalloccalls
		)
{
	(v_vkbufcreatinfo).sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	(v_vkbufcreatinfo).pNext = 0;
	(v_vkbufcreatinfo).usage = (vkbufuseflags);
	(v_vkbufcreatinfo).pQueueFamilyIndices = 0;
	(v_vkbufcreatinfo).queueFamilyIndexCount = 0;
	(v_vkbufcreatinfo).sharingMode = vksharemode;
	(v_vkbufcreatinfo).flags = 0;
	(v_vkbufcreatinfo).size = (sz);

	if(((r_vkres) = vkCreateBuffer(
					vkdev
					, &(v_vkbufcreatinfo)
					, vkalloccalls
					, &(r_vkbuf)
				      )) != VK_SUCCESS
	  )
	{
		assert(!"Failed vkCreateBuffer");
		CMDONERR;
	}

	unless((r_vkbuf)) {
		assert(!"vkCreateBuffer went null");
		(v_reterr) |= ae2f_errGlob_ALLOC_FAILED;
		CMDONERR;
	}
}

ae2f_MAC(CMDONERR, ) _ae2fVK_AnnSlpMkAllocVKMem_imp(
		const size_t			sz,
		VkResult			r_vkres,
		VkBuffer			r_vkbuf,
		VkDeviceMemory			r_vkdevmem,

		ae2f_err_t			v_errbit,
		VkBufferCreateInfo		v_vkbufcreatinfo,
		VkMemoryAllocateInfo		v_memallocinfo,

		const VkBufferUsageFlags	vkbufuseflags,
		const VkSharingMode		vksharemode,
		const VkDevice			vkdev,

		VkMemoryRequirements		v_vkmemreq,

		ae2f_opt const VkAllocationCallbacks*	vkalloccalls,

		unsigned char				v_memtypeidx,
		const unsigned char			memtypeidx_invalid_minus_one,
		const VkMemoryPropertyFlagBits		vkpropflag_req,
		const VkPhysicalDeviceMemoryProperties	vkphydevmemprops
		)
{
	__ae2fVK_AnnSlpMkCreatBuf(
			CMDONERR
			, sz
			, r_vkres
			, r_vkbuf
			, v_vkbufcreatinfo
			, v_errbit
			, vkbufuseflags
			, vksharemode
			, vkdev
			, vkalloccalls
			);

	__ae2fVK_AnnSlpMkFndMemProp_imp(
			CMDONERR
			, v_memtypeidx
			, v_errbit
			, memtypeidx_invalid_minus_one
			, vkpropflag_req
			, vkphydevmemprops
			);

	vkGetBufferMemoryRequirements(
			vkdev
			, r_vkbuf
			, &(v_vkmemreq)
			);

	unless((v_vkmemreq).size >= (v_vkbufcreatinfo).size) {
		assert(!"Buffer size exceeds memory requirements");
		(v_errbit) |= ae2f_errGlob_IMP_NOT_FOUND;
		CMDONERR;
	}

	unless((v_vkmemreq).size <= 
			(vkphydevmemprops).memoryHeaps[(vkphydevmemprops).memoryTypes[v_memtypeidx].heapIndex].size)
	{
		assert(!"Requirement size exceeds memory heap size.");
		(v_errbit) |= ae2f_errGlob_IMP_NOT_FOUND;
		CMDONERR;
	}

	(v_memallocinfo).sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	(v_memallocinfo).allocationSize = (v_vkmemreq).size;
	(v_memallocinfo).memoryTypeIndex = (v_memtypeidx);
	(v_memallocinfo).pNext = NULL;

	if(((r_vkres) = vkAllocateMemory(
					vkdev
					, &(v_memallocinfo)
					, (vkalloccalls)
					, &(r_vkdevmem)
					)) != VK_SUCCESS)
	{
		assert(!"vkAllocateMemory has failed.");
		CMDONERR;
	}

	unless((r_vkdevmem)) {
		assert(!"vkAllocateMemory went null.");
		(v_errbit) |= ae2f_errGlob_ALLOC_FAILED;
		CMDONERR;
	}

	if(((r_vkres) = vkBindBufferMemory(
					vkdev
					, r_vkbuf
					, r_vkdevmem
					, 0
					)) != VK_SUCCESS)
	{
		assert(!"vkBindBufferMemory has failed.");
		CMDONERR;
	}
}

ae2f_MAC(CMDONERR, ) _ae2fVK_AnnSlpMkLoadPipeCreat_imp(
		ae2f_err_t				r_reterr,
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

	unless((pipelayout)[idx_pipelayout]) {
		assert(!"Pipelayout went null");
		(r_reterr) |= ae2f_errGlob_ALLOC_FAILED;
		CMDONERR;
	}

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

ae2f_MAC(CMDONERR, ) _ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp(
		VkPushConstantRange			v_vkpushconstrange,
		VkPipelineLayoutCreateInfo		v_vkpipelayoutcreatinfo,
		ae2f_err_t				r_reterr,
		ae2fVK_AnnSlp* const			iv_slpptr,

		const ae2fVK_eAnnSlpPipeLayouts		idx
		, const uint32_t			szrequired
		, const VkAllocationCallbacks* const	vkalloccalls
		, const VkDevice			vkdev
		)
{
	(v_vkpushconstrange).size
		= (szrequired);

	if((szrequired) & 0b11) {
		(v_vkpushconstrange).size
			= ((szrequired) + 4)
			& ae2f_static_cast(uint32_t, ~0b11);
	}

	if(((iv_slpptr)->m_vkres = vkCreatePipelineLayout(
					vkdev
					, &(v_vkpipelayoutcreatinfo)
					, (vkalloccalls)
					, &(iv_slpptr)->m_vkpipelayout[idx]
					)) != VK_SUCCESS)
	{
		assert(!"vkCreatePipelineLayout has failed.");
		CMDONERR;
	}

	unless((iv_slpptr)->m_vkpipelayout[idx])
	{
		assert(!"vkCreatePipelineLayout went null");
		(r_reterr) |= ae2f_errGlob_ALLOC_FAILED;
		CMDONERR;
	}
}


ae2f_MAC() _ae2fVK_AnnSlpCreatDescPool_imp(
		ae2fVK_AnnSlpCreatDescPool_t	v_descpool,
		ae2fVK_AnnSlp			iv_slp,

		VkDescriptorPool			r_vkdescpool,
		ae2f_err_t				r_reterr,

		const VkDescriptorPoolCreateFlagBits	i_vkdescpoolcreatflags,
		const VkDescriptorType			i_vkdesctype,
		const uint32_t				i_vkdescpoolsz_count,
		const uint32_t				i_vkmaxdescsets
		)
{
	__ae2fVK_AnnSlpCreatDescPoolVerbose_imp(
			(v_descpool).m_U0.m_vkdescpoolcreatinfo
			, (v_descpool).m_U0.m_vkdescpoolsz
			, r_vkdescpool
			, r_reterr
			, (iv_slp).m_vkres
			, (iv_slp).m_vkdev
			, (iv_slp).m_vkalloccalls
			, i_vkdescpoolcreatflags
			, i_vkdesctype
			, i_vkdescpoolsz_count
			, i_vkmaxdescsets
			);
}

ae2f_MAC() _ae2fVK_AnnSlpCreatDescPoolVerbose_imp(
		VkDescriptorPoolCreateInfo		v_vkdescpoolcreatinfo,
		VkDescriptorPoolSize			v_vkdescpoolsz,

		VkDescriptorPool			r_vkdescpool,
		ae2f_err_t				r_reterr,
		VkResult				r_vkres,

		const VkDevice				i_vkdev,
		const VkAllocationCallbacks* const	i_vkptrAlloc,
		const VkDescriptorPoolCreateFlagBits	i_vkdescpoolcreatflags,
		const VkDescriptorType			i_vkdesctype,
		const uint32_t				i_vkdescpoolsz_count,
		const uint32_t				i_vkmaxdescsets
		)
{
	(v_vkdescpoolsz).type = (i_vkdesctype);
	(v_vkdescpoolsz).descriptorCount = (i_vkdescpoolsz_count);

	(v_vkdescpoolcreatinfo).sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	(v_vkdescpoolcreatinfo).flags = 0;
	(v_vkdescpoolcreatinfo).pNext = NULL;
	(v_vkdescpoolcreatinfo).flags = (i_vkdescpoolcreatflags);
	(v_vkdescpoolcreatinfo).poolSizeCount = 1;
	(v_vkdescpoolcreatinfo).pPoolSizes = &(v_vkdescpoolsz);
	(v_vkdescpoolcreatinfo).maxSets = (i_vkmaxdescsets); 

	if(((r_vkres) = vkCreateDescriptorPool(
					(i_vkdev)
					, &(v_vkdescpoolcreatinfo)
					, (i_vkptrAlloc)
					, &(r_vkdescpool)
					)) != VK_SUCCESS)
	{
		assert(!"vkCreateDescriptorPool has failed.");
	}

	unless((r_vkdescpool)) {
		assert(!"vkCreateDescriptorPool went null.");
		(r_reterr) |= ae2f_errGlob_ALLOC_FAILED;
	}
}

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
		, const VkPhysicalDeviceMemoryProperties		vkmemprops
		, ae2f_opt VkAllocationCallbacks* const			vkalloccalls,

		ae2f_opt const char* const				vkcldeclaration
		, ae2f_opt const char* const				vkcldefinition
) {
	__ae2fVK_AnnSlpMk_imp_V(
			ae2f_float_t
			, v_mk, weight_opt, bias_opt, cache_opt
			, inc, outc, prm_offset
			, extra, act, actderiv, lossderiv
			, learningrate, learningrate_bias
			, vkdev, vkmemprops, vkalloccalls
			, vkcldeclaration, vkcldefinition
			);
}

#if __ae2f_MACRO_GENERATED
#else
typedef ae2f_float_t cllocfloat_t;
#endif

/**
 * @brief
 * Make slp with OpenCL compute shader.
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
ae2f_MAC(cllocfloat_t, ) _ae2fVK_AnnSlpMk_imp_V(
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
		, const VkPhysicalDeviceMemoryProperties		vkmemprops
		, ae2f_opt VkAllocationCallbacks* const			vkalloccalls,

		ae2f_opt const char* const				vkcldeclaration
		, ae2f_opt const char* const				vkcldefinition
)
{
	assert((vkdev) && "Vulkan device null check");
	assert((lossderiv) && "lossderiv must be specified.");

	(v_mk).m_reterr = ae2f_errGlob_OK;

	do {
		__ae2f_AnnSlpMk_imp(
				(v_mk).m_U0.m_base, (weight_opt)
				, (bias_opt), (cache_opt)
				, (inc), (outc)
				, ((prm_offset) + sizeof(ae2fVK_AnnSlp) - sizeof(ae2f_AnnSlp))
				, (extra)
				, (act), (actderiv), (lossderiv)
				, (learningrate), (learningrate_bias)
				);

		if((v_mk).m_U0.m_base.m_ptr->m_Slp[0].m_inc > UINT32_MAX) {
			assert(!"Input size is too large");
			(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
			break;
		}

		if((v_mk).m_U0.m_base.m_ptr->m_Slp[0].m_outc > UINT32_MAX) {
			assert(!"Output size is too large");
			(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
			break;
		}

		(v_mk).m_U0.m_alter.m_ptr->m_vkdev = vkdev;
		(v_mk).m_U0.m_alter.m_ptr->m_vkalloccalls = vkalloccalls;

		/** Allocate the global memory */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break;
				, __ae2fVK_AnnSlpGlobSz(inc, outc)
				, (v_mk).m_U0.m_alter.m_ptr->m_vkres
				, (v_mk).m_U0.m_alter.m_ptr->m_vkglobbuf
				, (v_mk).m_U0.m_alter.m_ptr->m_vkglobdevmem
				, (v_mk).m_reterr
				, (v_mk).m_U1.m_vkbufcreatinfo
				, (v_mk).m_U1.m_vkmemallocinfo
				, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
				, VK_SHARING_MODE_EXCLUSIVE
				, (vkdev)
				, (v_mk).m_U2.m_vkmemreqs
				, (vkalloccalls)
				, (v_mk).m_U0.m_alter.m_i	/* v_memtypeidx */
				, UCHAR_MAX
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				, vkmemprops
				);

		/** the local memory */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break;
				, sizeof(cllocfloat_t) * ((outc))
				, (v_mk).m_U0.m_alter.m_ptr->m_vkres
				, (v_mk).m_U0.m_alter.m_ptr->m_vklocbuf
				, (v_mk).m_U0.m_alter.m_ptr->m_vklocdevmem
				, (v_mk).m_reterr
				, (v_mk).m_U1.m_vkbufcreatinfo
				, (v_mk).m_U1.m_vkmemallocinfo
				, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
				, VK_SHARING_MODE_EXCLUSIVE
				, (vkdev)
				, (v_mk).m_U2.m_vkmemreqs
				, (vkalloccalls)
				, (v_mk).m_U0.m_alter.m_i	/* v_memtypeidx */
				, UCHAR_MAX
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
				, vkmemprops
				);

		(v_mk).m_U2.m_S0.m_glob.binding = 0;

		(v_mk).m_U2.m_S0.m_glob.descriptorType
			= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		(v_mk).m_U2.m_S0.m_glob.descriptorCount = 1;

		(v_mk).m_U2.m_S0.m_glob.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		(v_mk).m_U2.m_S0.m_glob.pImmutableSamplers = NULL;

		(v_mk).m_U2.m_S0.m_loc = (v_mk).m_U2.m_S0.m_glob;
		(v_mk).m_U2.m_S0.m_loc.binding = 1;


		(v_mk).m_U1.m_vkdescsetlayoutcreatinfo.pBindings = (v_mk).m_U2.m_vkdescsetlayoutbind;
		(v_mk).m_U1.m_vkdescsetlayoutcreatinfo.bindingCount = 1;
		(v_mk).m_U1.m_vkdescsetlayoutcreatinfo.flags = 0;
		(v_mk).m_U1.m_vkdescsetlayoutcreatinfo.pNext = NULL;
		(v_mk).m_U1.m_vkdescsetlayoutcreatinfo.sType
			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		if(((v_mk).m_U0.m_alter.m_ptr->m_vkres = vkCreateDescriptorSetLayout(
						vkdev
						, &(v_mk).m_U1.m_vkdescsetlayoutcreatinfo
						, (vkalloccalls)
						, &(v_mk).m_U0.m_alter.m_ptr->m_vkdescsetlayout
						[ae2fVK_eAnnSlpDescLayouts_kPredict]
						)) != VK_SUCCESS) {
			assert(!"vkCreateDescriptorSetLayout 0 has failed");
			break;
		}

		unless((v_mk).m_U0.m_alter.m_ptr->m_vkdescsetlayout[0]) {
			assert(!"vkCreateDescriptorSetLayout 0 went null");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		(v_mk).m_U1.m_vkdescsetlayoutcreatinfo.bindingCount = 2;

		if(((v_mk).m_U0.m_alter.m_ptr->m_vkres = vkCreateDescriptorSetLayout(
						vkdev
						, &(v_mk).m_U1.m_vkdescsetlayoutcreatinfo
						, (vkalloccalls)
						, &(v_mk).m_U0.m_alter.m_ptr->m_vkdescsetlayout
						[ae2fVK_eAnnSlpDescLayouts_kTrain]
						)) != VK_SUCCESS) {
			assert(!"vkCreateDescriptorSetLayout 1 has failed");
			break;
		}

		unless((v_mk).m_U0.m_alter.m_ptr->m_vkdescsetlayout[1]) {
			assert(!"vkCreateDescriptorSetLayout 1 went null");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		(v_mk).m_U1.m_vkpushconstrange.offset = 0;
		(v_mk).m_U1.m_vkpushconstrange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		(v_mk).m_U2.m_vkpipelayoutcreatinfo.sType 
			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.setLayoutCount = 1;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pushConstantRangeCount = 1;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pPushConstantRanges
			= &(v_mk).m_U1.m_vkpushconstrange;
		(v_mk).m_U2.m_vkpipelayoutcreatinfo.pSetLayouts
			= (v_mk).m_U0.m_alter.m_ptr->m_vkdescsetlayout;

		__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp(
				break, 
				(v_mk).m_U1.m_vkpushconstrange
				, (v_mk).m_U2.m_vkpipelayoutcreatinfo
				, (v_mk).m_reterr
				, (v_mk).m_U0.m_alter.m_ptr
				, ae2fVK_eAnnSlpPipeLayouts_kPredict
				, sizeof(size_t)
				, vkalloccalls
				, vkdev
				);

		if ((v_mk).m_U0.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 0 has failed.");
			break;
		}

		__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp(
				break, 
				(v_mk).m_U1.m_vkpushconstrange
				, (v_mk).m_U2.m_vkpipelayoutcreatinfo
				, (v_mk).m_reterr
				, (v_mk).m_U0.m_alter.m_ptr
				, ae2fVK_eAnnSlpPipeLayouts_kTrain
				, sizeof(ae2f_float_t) * 2
				, vkalloccalls
				, vkdev
				);

		if ((v_mk).m_U0.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 1 has failed.");
			break;
		}

		assert((vkcldeclaration) && "vkcldeclaration is null");
		assert((vkcldefinition) && "vkcldefinition is null");

		{
			__ae2fVK_AnnSlpMkCLSPVVerbose_imp(
					(v_mk).m_U3.m_openclsrc_v
					, (v_mk).m_reterr
					, (vkcldeclaration)
					, ae2fVK_AnnSlpSHADER
					, (vkcldefinition)
					);

			if((v_mk).m_reterr) {
				assert(!"__ae2fVK_AnnSlpMkCLSPV_imp has failed.");
				break;
			}

			assert((v_mk).m_U3.m_openclsrc);

			(v_mk).m_U2.m_log = ae2f_static_cast(char*, NULL);
			(v_mk).m_U1.m_spirv = NULL;
			(v_mk).m_U0.m_spirv_len = 0;

			assert((v_mk).m_U0.m_alter.m_ptr);

			switch(clspvCompileFromSourcesString(
						1
						, ae2f_reinterpret_cast(const size_t*, NULL)
						, ae2f_const_cast(
							const char** ae2f_restrict
							, (&(v_mk).m_U3.m_openclsrc)
							)
						,""
						, &(v_mk).m_U1.m_spirv_c
						, &(v_mk).m_U0.m_spirv_len
						, &(v_mk).m_U2.m_log
						))
			{
				case CLSPV_ERROR:
					(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
					assert(!"clspvCompileFromSourcesString has failed.");

					break;
				case CLSPV_OUT_OF_HOST_MEM:
					(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
					assert(!"clspvCompileFromSourcesString has failed its allocation.");

					break;

				case CLSPV_INVALID_ARG:
					(v_mk).m_reterr |= ae2f_errGlob_WRONG_OPERATION;
					assert(!"clspvCompileFromSourcesString found arguments invalid.");

					break;

				case CLSPV_SUCCESS: break;
			}

			free((v_mk).m_U3.m_openclsrc);
			free((v_mk).m_U2.m_log);
		}

		assert((v_mk).m_U0.m_base.m_ptr && "clspvCompileFromSourcesString has done something weird");

		(v_mk).m_U2.m_vkshadermodcreatinfo.pNext = NULL;
		(v_mk).m_U2.m_vkshadermodcreatinfo.codeSize
			= (v_mk).m_U0.m_spirv_len;
		(v_mk).m_U2.m_vkshadermodcreatinfo.pCode
			= (v_mk).m_U1.m_spirv_u32;
		(v_mk).m_U2.m_vkshadermodcreatinfo.flags = 0;
		(v_mk).m_U2.m_vkshadermodcreatinfo.sType
			= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		if(((v_mk).m_U0.m_alter.m_ptr->m_vkres = vkCreateShaderModule(
						vkdev
						, &(v_mk).m_U2.m_vkshadermodcreatinfo
						, (vkalloccalls)
						, &(v_mk).m_U0.m_alter.m_ptr->m_vkshadermodule
						)) != VK_SUCCESS)
		{
			assert(!"vkCreateShaderModule has failed.");
			break;
		}

		unless((v_mk).m_U0.m_alter.m_ptr->m_vkshadermodule) 
		{
			assert(!"vkCreateShaderModule has produced NULL.");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		free((v_mk).m_U1.m_spirv);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_U2.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_U0.m_alter.m_ptr->m_vkshadermodule
				, "kFollow"
				, (ae2fVK_eAnnSlpPipes_kFollow)
				, (ae2fVK_eAnnSlpPipeLayouts_kTrain)
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_U2.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_U0.m_alter.m_ptr->m_vkshadermodule
				, "kFit"
				, (ae2fVK_eAnnSlpPipes_kFit)
				, (ae2fVK_eAnnSlpPipeLayouts_kTrain)
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_U2.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_U0.m_alter.m_ptr->m_vkshadermodule
				, "kPredict"
				, ae2fVK_eAnnSlpPipes_kPredict
				, ae2fVK_eAnnSlpPipeLayouts_kPredict
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_U2.m_vkcomputepipecreatinfo
				, (v_mk).m_U0.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_U0.m_alter.m_ptr->m_vkshadermodule
				, "kTrain"
				, (ae2fVK_eAnnSlpPipes_kTrain)
				, (ae2fVK_eAnnSlpPipeLayouts_kTrain)
				);

		if (((v_mk).m_U0.m_alter.m_ptr->m_vkres
					= vkCreateComputePipelines(
						(vkdev)
						, VK_NULL_HANDLE
						, ae2fVK_eAnnSlpPipes_LEN
						, (v_mk).m_U2.m_vkcomputepipecreatinfo
						, (vkalloccalls)
						, (v_mk).m_U0.m_alter.m_ptr->m_vkpipeline
						)
		    ) != VK_SUCCESS)
		{
			assert(!"vkCreateComputePipelines has failed.");
			(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
			break;
		}
	} while(0);

	assert((v_mk).m_reterr == ae2f_errGlob_OK && "Returned error flag has set up");
	if((v_mk).m_U0.m_alter.m_ptr->m_vkres) {
		assert(!"Vulkan result check for validation");
		(v_mk).m_reterr |= ae2f_errGlob_NFOUND | ae2f_errGlob_ALLOC_FAILED;
	}
}

/** 
 * @param r_handle must be destroyed by free().
 * @param i_second_raw must be guaranteed to be a raw stirng. 
 * */
ae2f_MAC() _ae2fVK_AnnSlpMkCLSPVVerbose_imp(
		void* ae2f_restrict		r_handle,
		ae2f_err_t		ir_err,

		const char* ae2f_restrict	i_first,
		const char* ae2f_restrict	i_second,
		const char* ae2f_restrict	i_third
		) 
{
	if(!((r_handle) = calloc(
					1
					, strlen(i_first) + sizeof(
						i_second
						) + strlen(i_third)
				)))
	{
		assert(!"_ae2fVK_AnnSlpMkCLSPV_imp has failed.");
		(ir_err) |= ae2f_errGlob_ALLOC_FAILED;
	}

	else {
		strcpy(ae2f_reinterpret_cast(char*, r_handle), i_first);
		strcat(ae2f_reinterpret_cast(char*, r_handle), i_second);
		strcat(ae2f_reinterpret_cast(char*, r_handle), i_third);
	}
}

/** Map's usually for output */
ae2f_MAC() _ae2fVK_AnnSlpMap_imp(
		ae2f_err_t r_err,

		ae2fVK_AnnSlp	slp,
		ae2f_float_t** ae2f_restrict const data
		)
{
	if((slp).m_vkres != VK_SUCCESS) {
		assert(!"Result flag is not valid.");
	}

	assert((slp).m_vkdev);
	assert((slp).m_vkglobdevmem);
	assert(r_err == ae2f_errGlob_OK && "Previous status is bad.");

	do {
		if(((slp).m_vkres = vkMapMemory(
						(slp).m_vkdev
						, (slp).m_vkglobdevmem
						, 0, VK_WHOLE_SIZE
						, 0
						, ae2f_reinterpret_cast(void** ae2f_restrict, data)
					       )) != VK_SUCCESS) 
		{
			assert(!"vkMapMemory has failed.");
			break;
		}

		unless(*(data)) {
			assert(!"vkMapMemory went null.");
			(r_err) |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}
	} while(0);
}

#if !__ae2f_MACRO_GENERATED
#define T int
#endif

ae2f_MAC(T,) _ae2fVK_AnnSlpMapRangedGeneric_imp(
		VkMappedMemoryRange	v_vkmapmemrange,
		ae2fVK_AnnSlp		iv_slp,

		ae2f_err_t		r_err,
		T*			r_ptr,

		const VkDeviceSize	i_off,
		const VkDeviceSize	i_sz
		)
{
	assert((iv_slp).m_vkres == VK_SUCCESS);
	assert((iv_slp).m_vkdev);
	assert((iv_slp).m_vkglobdevmem);
	assert((r_err) == ae2f_errGlob_OK && "Previous status is bad.");

	do {
		if(((iv_slp).m_vkres = vkMapMemory(
						(iv_slp).m_vkdev
						, (iv_slp).m_vkglobdevmem
						, (v_vkmapmemrange).offset = (i_off)
						,  (v_vkmapmemrange).size = (i_sz)
						,0
						, ae2f_reinterpret_cast(void**, &(r_ptr))
						)
		   ) != VK_SUCCESS)
		{
			assert(!"vkMapMemory has failed.");
			break;
		}

		unless((r_ptr)) {
			assert(!"vkMapMemory went null.");
			(r_err) |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		(v_vkmapmemrange).memory = (iv_slp).m_vkglobdevmem;
		(v_vkmapmemrange).pNext = NULL;
		(v_vkmapmemrange).sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;

		if (((iv_slp).m_vkres = vkInvalidateMappedMemoryRanges(
						(iv_slp).m_vkdev
						, 1
						, &(v_vkmapmemrange)
						)) != VK_SUCCESS)
		{
			assert(!"vkInvalidateMappedMemoryRanges has failed.");
			break;
		}
	} while(0);

	unless((iv_slp).m_vkres == VK_SUCCESS) {
		(r_err) |= ae2f_errGlob_NFOUND;
	}
}

#undef T

ae2f_MAC() _ae2fVK_AnnSlpUnMap_imp(const ae2fVK_AnnSlp slp) {
	assert((slp).m_vkdev);
	assert((slp).m_vkglobdevmem);
	vkUnmapMemory((slp).m_vkdev, (slp).m_vkglobdevmem);
}

ae2f_MAC() _ae2fVK_AnnSlpUnMapRanged_imp(
		ae2fVK_AnnSlpUnMap_t		v_out,
		ae2fVK_AnnSlp				vi_slp,
		const VkDeviceSize			i_off,
		const VkDeviceSize			i_sz
		)
{
	assert((vi_slp).m_vkres == VK_SUCCESS);

	(v_out).memory = (vi_slp).m_vkglobdevmem;
	(v_out).offset = (i_off);
	(v_out).pNext = NULL;
	(v_out).size = (i_sz);
	(v_out).sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;

	do {
		if (((vi_slp).m_vkres = vkFlushMappedMemoryRanges(
						(vi_slp).m_vkdev
						, 1
						, &(v_out)
						)) != VK_SUCCESS)
		{
			assert(!"vkFlushMappedMemoryRanges has failed.");
			break;
		}

		__ae2fVK_AnnSlpUnMap_imp(vi_slp);
	} while(0);
}

ae2f_MAC() _ae2fVK_AnnSlpClean_imp(
		const ae2fVK_AnnSlp	block
		)
{
	assert((block).m_vkdev && "Double free is suspected.");

	if((block).m_vkdev) {
		if((block).m_vkglobbuf) {
			vkDestroyBuffer(
					(block).m_vkdev
					, (block).m_vkglobbuf
					, (block).m_vkalloccalls
				       );
		}

		if((block).m_vklocbuf) {
			vkDestroyBuffer(
					(block).m_vkdev
					, (block).m_vklocbuf
					, (block).m_vkalloccalls
				       );
		}

		if((block).m_vkglobdevmem) {
			vkFreeMemory(
					(block).m_vkdev
					, (block).m_vkglobdevmem
					, (block).m_vkalloccalls
				    );
		}

		if((block).m_vklocdevmem) {
			vkFreeMemory(
					(block).m_vkdev
					, (block).m_vklocdevmem
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

		if((block).m_vkdescsetlayout[1]) {
			vkDestroyDescriptorSetLayout(
					(block).m_vkdev
					, (block).m_vkdescsetlayout[1]
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
}

/** @brief Unmaps Weight and Bias */
ae2f_MAC() _ae2fVK_AnnSlpWBUnMap_imp(
		ae2fVK_AnnSlpUnMap_t		v_out,
		ae2fVK_AnnSlp 				vi_slp
		)
{
	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_out
			, vi_slp
			, __ae2fVK_AnnSlpWeightOff((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			, (__ae2fVK_AnnSlpWeightSz((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			+ __ae2fVK_AnnSlpBiasSz((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc))

			);
}

/** @brief Maps Weight and Bias */
ae2f_MAC() _ae2fVK_AnnSlpWBMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_float_t** ae2f_restrict const ir_ptrweight
		, ae2f_float_t** ae2f_restrict const ir_ptrbias
		)
{
	ae2f_err_t			v_err = 0;
	ae2fVK_AnnSlpMapRangedGeneric_t	v_map;

	if(slp) {
		assert((ir_ptrweight));

		__ae2fVK_AnnSlpMapRangedGeneric_imp(
				ae2f_float_t
				, v_map
				, (slp)[0]
				, *(r_err)
				, *(ir_ptrweight)
				, __ae2fVK_AnnSlpWeightOff((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				, (__ae2fVK_AnnSlpWeightSz((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				+ __ae2fVK_AnnSlpBiasSz((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc))
				);
	} else {
		assert(!"Got null pointer from slp");
		(v_err) |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(ae2f_errGlob_OK == v_err) {
		if(ir_ptrbias) {
			*(ir_ptrbias) = 
				*(ir_ptrweight)
				+ (slp)->m_slp.m_Slp[0].m_inc * (slp)->m_slp.m_Slp[0].m_outc;
		}
	}

	if(r_err) {
		*(r_err) |= (v_err);
		assert(v_err == ae2f_errGlob_OK);
	}
}

ae2f_MAC() _ae2fVK_AnnSlpIOUnMap_imp(
		ae2fVK_AnnSlpUnMap_t		v_out,
		ae2fVK_AnnSlp 			vi_slp
		)
{

	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_out
			, vi_slp
			, __ae2fVK_AnnSlpInpOff((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			, __ae2fVK_AnnSlpInpSz((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			+ __ae2fVK_AnnSlpOutSz((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			);
}



ae2f_MAC() _ae2fVK_AnnSlpIOMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_opt ae2f_float_t** ae2f_restrict const ir_ptrinp
		, ae2f_opt ae2f_float_t** ae2f_restrict const ir_ptrout
		)
{
	ae2f_err_t		v_err = 0;
	ae2fVK_AnnSlpMapRangedGeneric_t	v_map;

	assert(ir_ptrinp);

	if((slp) && (ir_ptrinp)) {
		__ae2fVK_AnnSlpMapRangedGeneric_imp(
				ae2f_float_t
				, v_map
				, (*(slp))
				, v_err
				, (*(ir_ptrinp))
				, __ae2fVK_AnnSlpInpOff((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				, __ae2fVK_AnnSlpInpSz((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				+ __ae2fVK_AnnSlpOutSz((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(ae2f_errGlob_OK == v_err) {
		if(ir_ptrout) {
			*(ir_ptrout) = 
				(*(ir_ptrinp))
				+ ((slp)->m_slp.m_Slp[0].m_inc);
		}
	}

	if(r_err) {
		*(r_err) |= (v_err);
	}
}

ae2f_MAC() _ae2fVK_AnnSlpDeltaUnMap_imp(
		ae2fVK_AnnSlpUnMap_t		v_out,
		ae2fVK_AnnSlp 				vi_slp

		)
{

	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_out
			, vi_slp
			, __ae2fVK_AnnSlpDeltaOff((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			, __ae2fVK_AnnSlpDeltaSz((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			);
}

ae2f_MAC() _ae2fVK_AnnSlpDeltaMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_opt ae2f_float_t** ae2f_restrict const ir_ptrdelta
		)
{
	ae2f_err_t			v_err = 0;
	ae2fVK_AnnSlpMapRangedGeneric_t	v_map;

	if((slp) && (ir_ptrdelta)) {
		__ae2fVK_AnnSlpMapRangedGeneric_imp(
				ae2f_float_t
				, v_map
				, *(slp)
				, *(r_err)
				, *(ir_ptrdelta)
				, __ae2fVK_AnnSlpDeltaOff((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				, __ae2fVK_AnnSlpDeltaSz((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if((v_err)) {
		assert(!"__ae2fVK_AnnSlpMapRangedGeneric_imp has failed.");
		(r_err) && (*(r_err) |= (v_err));
	}
}

ae2f_MAC() _ae2fVK_AnnSlpGoalUnMap_imp(
		ae2fVK_AnnSlpUnMap_t		v_out,
		ae2fVK_AnnSlp 				vi_slp
		)
{
	__ae2fVK_AnnSlpUnMapRanged_imp(
			v_out
			, vi_slp
			, __ae2fVK_AnnSlpGoalOff((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			, __ae2fVK_AnnSlpGoalSz((vi_slp).m_slp.m_Slp[0].m_inc, (vi_slp).m_slp.m_Slp[0].m_outc)
			);
}

ae2f_MAC() _ae2fVK_AnnSlpGoalMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_opt ae2f_float_t** ae2f_restrict const ir_ptrgoal
		)
{
	ae2f_err_t			v_err = 0;
	ae2fVK_AnnSlpMapRangedGeneric_t	v_map;

	if((slp) && (ir_ptrgoal)) {
		__ae2fVK_AnnSlpMapRangedGeneric_imp(
				ae2f_float_t
				, v_map
				, *(slp)
				, *(r_err)
				, (*ir_ptrgoal)
				, __ae2fVK_AnnSlpGoalOff((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				, __ae2fVK_AnnSlpGoalSz((slp)->m_slp.m_Slp[0].m_inc, (slp)->m_slp.m_Slp[0].m_outc)
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(v_err) {
		assert(!"__ae2fVK_AnnSlpMapRangedGeneric_imp has failed.");
		(r_err) && (*(r_err) |= (v_err));
	}
}

#endif
