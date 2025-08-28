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

#if __ae2f_MACRO_GENERATED

#define ae2fVK_AnnSlpSHADER \
#include "./cl/Slp.auto.cl.impl"

#endif

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

/**
 * @def __ae2fVK_AnnSlpMkOnePipeLayout_imp
 *
 * @brief Make one pipeline layout with given push-constant size.
 *
 * @tparam CMDONERR
 * Break logic code for exception handeling.
 *
 * @param v_mk			: {ae2fVK_AnnSlpMk_t&}
 * @param i_idx			: {const ae2fVK_eAnnSlpPipeLayouts}
 * @param i_szrequired		: {const size_t}
 * @param iv_vkalloccalls	: {const VkAllocationCallbacks* const}
 * @param i_vkdev		: {const VkDevice}
 * */
#define __ae2fVK_AnnSlpMkOnePipeLayout_imp(CMDONERR, v_mk, i_idx, i_szrequired, iv_vkalloccalls, i_vkdev) \
	__ae2fVK_AnnSlpMkOnePipeLayoutVerbose_imp( \
			CMDONERR \
			, (v_mk).m_vkstack.m_layout.m_pushconstant \
			, (v_mk).m_vkinfo.m_pipelayoutcreat \
			, (v_mk).m_reterr \
			, (v_mk).m_union.m_alter.m_ptr \
			, (i_idx), (i_szrequired), (iv_vkalloccalls), (i_vkdev) \
			);

ae2f_MAC(CMDONERR, ) _ae2fVK_AnnSlpMkCreatDescPool(
		VkDescriptorPoolCreateInfo		v_vkdescpoolcreatinfo,
		VkDescriptorPoolSize			v_vkdescpoolsz,

		VkDescriptorPool			r_vkdescpool,
		ae2f_err_t				r_reterr,
		VkResult				r_vkres,

		const VkDevice				i_vkdev,
		const VkAllocationCallbacks* const	i_vkptrAlloc,
		const VkDescriptorPoolCreateFlagBits	i_vkdescpoolcreatflags,
		const VkDescriptorType			i_vkdesctype,
		const uint32_t				i_vkdescpoolsz_count
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
	(v_vkdescpoolcreatinfo).maxSets = 1; /** I actually forgot to initialise this? lol;; */

	if(((r_vkres) = vkCreateDescriptorPool(
					(i_vkdev)
					, &(v_vkdescpoolcreatinfo)
					, (i_vkptrAlloc)
					, &(r_vkdescpool)
					)) != VK_SUCCESS)
	{
		assert(!"vkCreateDescriptorPool has failed.");
		CMDONERR;
	}

	unless((r_vkdescpool)) {
		assert(!"vkCreateDescriptorPool went null.");
		(r_reterr) |= ae2f_errGlob_ALLOC_FAILED;
		CMDONERR;
	}
}

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
)
{
	assert((vkdev) && "Vulkan device null check");
	assert((lossderiv) && "lossderiv must be specified.");

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

		(v_mk).m_union.m_base.m_alloccount = 
			(v_mk).m_union.m_base.m_alloccount
			- sizeof(ae2f_AnnSlp)
			+ ae2f_CmpGetGt(
					(sizeof(ae2f_float_t) * (outc) * 2)
					, (sizeof(ae2f_float_t) * ((outc) + (inc)))
				       )
			; /** weight, bias, deltacache */

		(v_mk).m_union.m_alter.m_ptr->m_vkdev = vkdev;
		(v_mk).m_union.m_alter.m_ptr->m_vkalloccalls = vkalloccalls;

		/** Allocate the global memory */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break;
				, (v_mk).m_union.m_base.m_alloccount
				, (v_mk).m_union.m_alter.m_ptr->m_vkres
				, (v_mk).m_union.m_alter.m_ptr->m_vkglobbuf
				, (v_mk).m_union.m_alter.m_ptr->m_vkglobdevmem
				, (v_mk).m_reterr
				, (v_mk).m_vkinfo.m_buf
				, (v_mk).m_vkinfo.m_alloc
				, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
				, VK_SHARING_MODE_EXCLUSIVE
				, (vkdev)
				, (v_mk).m_vkstack.m_memreq
				, (vkalloccalls)
				, (v_mk).m_union.m_alter.m_i	/* v_memtypeidx */
				, UCHAR_MAX
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				, vkmemprops
				);

		/** the local memory */
		__ae2fVK_AnnSlpMkAllocVKMem_imp(
				break;
				, sizeof(ae2f_float_t) * ((outc))
				, (v_mk).m_union.m_alter.m_ptr->m_vkres
				, (v_mk).m_union.m_alter.m_ptr->m_vklocbuf
				, (v_mk).m_union.m_alter.m_ptr->m_vklocdevmem
				, (v_mk).m_reterr
				, (v_mk).m_vkinfo.m_buf
				, (v_mk).m_vkinfo.m_alloc
				, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
				, VK_SHARING_MODE_EXCLUSIVE
				, (vkdev)
				, (v_mk).m_vkstack.m_memreq
				, (vkalloccalls)
				, (v_mk).m_union.m_alter.m_i	/* v_memtypeidx */
				, UCHAR_MAX
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
				, vkmemprops
				);

		(v_mk).m_vkstack.m_layout.m_bind[0].binding = 0;
		(v_mk).m_vkstack.m_layout.m_bind[0].descriptorType
			= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		(v_mk).m_vkstack.m_layout.m_bind[0].descriptorCount = 1;
		(v_mk).m_vkstack.m_layout.m_bind[0].stageFlags
			= VK_SHADER_STAGE_COMPUTE_BIT;
		(v_mk).m_vkstack.m_layout.m_bind[0].pImmutableSamplers = NULL;

		(v_mk).m_vkstack.m_layout.m_bind[1]
			= (v_mk).m_vkstack.m_layout.m_bind[0];
		(v_mk).m_vkstack.m_layout.m_bind[1].binding = 1;

		(v_mk).m_vkstack.m_layout.m_creatinfo.pBindings
			= (v_mk).m_vkstack.m_layout.m_bind;
		(v_mk).m_vkstack.m_layout.m_creatinfo.bindingCount = 1;
		(v_mk).m_vkstack.m_layout.m_creatinfo.flags
			= 0;

		(v_mk).m_vkstack.m_layout.m_creatinfo.pNext = NULL;

		(v_mk).m_vkstack.m_layout.m_creatinfo.sType
			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreateDescriptorSetLayout(
						vkdev
						, &(v_mk).m_vkstack.m_layout.m_creatinfo
						, (vkalloccalls)
						, (v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout
						)) != VK_SUCCESS) {
			assert(!"vkCreateDescriptorSetLayout 0 has failed");
			break;
		}

		unless((v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout[0]) {
			assert(!"vkCreateDescriptorSetLayout 0 went null");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		(v_mk).m_vkstack.m_layout.m_creatinfo.bindingCount = 2;

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreateDescriptorSetLayout(
						vkdev
						, &(v_mk).m_vkstack.m_layout.m_creatinfo
						, (vkalloccalls)
						, (v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout + 1
						)) != VK_SUCCESS) {
			assert(!"vkCreateDescriptorSetLayout 1 has failed");
			break;
		}

		unless((v_mk).m_union.m_alter.m_ptr->m_vkdescsetlayout[1]) {
			assert(!"vkCreateDescriptorSetLayout 1 went null");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
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
				break;
				, v_mk
				, ae2fVK_eAnnSlpPipeLayouts_kPredict
				, sizeof(size_t)
				, vkalloccalls, vkdev
				);

		if ((v_mk).m_union.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 0 has failed.");
			break;
		}

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				break;
				, v_mk
				, ae2fVK_eAnnSlpPipeLayouts_kFollow
				, sizeof(ae2f_float_t) * 2
				, vkalloccalls, vkdev
				);

		if ((v_mk).m_union.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 1 has failed.");
			break;
		}

		__ae2fVK_AnnSlpMkOnePipeLayout_imp(
				break;
				, v_mk
				, ae2fVK_eAnnSlpPipeLayouts_kFit
				, sizeof(ae2f_float_t) * 2
				, vkalloccalls, vkdev
				);

		if ((v_mk).m_union.m_alter.m_ptr->m_vkres != VK_SUCCESS) {
			assert(!"__ae2fVK_AnnSlpMkOnePipeLayout_imp 2 has failed.");
			break;
		}

		assert((vkcldeclaration) && "vkcldeclaration is null");
		assert((vkcldefinition) && "vkcldefinition is null");

		{
			__ae2fVK_AnnSlpMkCLSPV_imp(
					(v_mk).m_clsrc.m_void
					, (v_mk).m_reterr
					, vkcldeclaration
					, vkcldefinition
					);

			if((v_mk).m_reterr) {
				assert(!"__ae2fVK_AnnSlpMkCLSPV_imp has failed.");
				break;
			}

			assert((v_mk).m_clsrc.m_void);

			(v_mk).m_unused = ae2f_static_cast(char*, NULL);
			(v_mk).m_clout_len = 0;
			(v_mk).m_clout.m_void = NULL;

			if(((v_mk).m_vkstack.m_isgood = clspvCompileFromSourcesString(
							1
							, ae2f_reinterpret_cast(const size_t*, NULL)
							, ae2f_const_cast(
								const char** restrict
								, (&(v_mk).m_clsrc.m_char)
								)
							,""
							"-pod-pushconstant "
							"-cl-fast-relaxed-math "
							"-cl-single-precision-constant"
							, &(v_mk).m_clout.m_char
							, &(v_mk).m_clout_len
							, &(v_mk).m_unused
							)) != CLSPV_SUCCESS)
			{
				assert(!"clspvCompileFromSourcesString has failed.");
				(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
				break;
			}

			free((v_mk).m_clsrc.m_void);
		}

		assert((v_mk).m_unused);
		assert((v_mk).m_union.m_base.m_ptr && "clspvCompileFromSourcesString has done something weird");

		(v_mk).m_vkinfo.m_shadermodulecreat.pNext = NULL;
		(v_mk).m_vkinfo.m_shadermodulecreat.codeSize
			= (v_mk).m_clout_len;

		(v_mk).m_vkinfo.m_shadermodulecreat.pCode
			= (v_mk).m_clout.m_wrds;

		(v_mk).m_vkinfo.m_shadermodulecreat.pNext = NULL;
		(v_mk).m_vkinfo.m_shadermodulecreat.flags
			= 0;

		(v_mk).m_vkinfo.m_shadermodulecreat.sType
			= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		assert((v_mk).m_vkinfo.m_shadermodulecreat.codeSize);
		assert((v_mk).m_vkinfo.m_shadermodulecreat.pCode);

		if(((v_mk).m_union.m_alter.m_ptr->m_vkres = vkCreateShaderModule(
						vkdev
						, &(v_mk).m_vkinfo.m_shadermodulecreat
						, (vkalloccalls)
						, &(v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
						)) != VK_SUCCESS)
		{
			assert(!"vkCreateShaderModule has failed.");
			break;
		}

		unless((v_mk).m_union.m_alter.m_ptr->m_vkshadermodule) 
		{
			assert(!"vkCreateShaderModule has produced NULL.");
			(v_mk).m_reterr |= ae2f_errGlob_ALLOC_FAILED;
			break;
		}

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kFollow"
				, (ae2fVK_eAnnSlpPipes_kFollow)
				, (ae2fVK_eAnnSlpPipeLayouts_kFollow)
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kFit"
				, (ae2fVK_eAnnSlpPipes_kFit)
				, (ae2fVK_eAnnSlpPipeLayouts_kFit)
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_pipecreat
				, (v_mk).m_union.m_alter.m_ptr->m_vkpipelayout
				, (v_mk).m_union.m_alter.m_ptr->m_vkshadermodule
				, "kPredict"
				, ae2fVK_eAnnSlpPipes_kPredict
				, ae2fVK_eAnnSlpPipeLayouts_kPredict
				);

		__ae2fVK_AnnSlpMkLoadPipeCreat_imp(
				break
				, (v_mk).m_reterr
				, (v_mk).m_pipecreat
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
			(v_mk).m_reterr |= ae2f_errGlob_NFOUND;
			break;
		}

		__ae2fVK_AnnSlpMkCreatDescPool(
				break
				, (v_mk).m_vkstack.m_creatdescpool.m_creatinfo /** v_vkcreatinfo */
				, (v_mk).m_vkstack.m_creatdescpool.m_sz

				, (v_mk).m_union.m_alter.m_ptr->m_vkdescpool[
				ae2fVK_eAnnSlpDescPools_kPredict
				] /** r_vkdescpool */
				, (v_mk).m_reterr
				, (v_mk).m_union.m_alter.m_ptr->m_vkres

				, (vkdev) /** i_vkdev */ 
				, (vkalloccalls) /** i_vkptrAlloc */
				, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT /** i_vkdescpoolcreatflags */
				, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER /** i_vkdesctype */
				, 1
				);

		__ae2fVK_AnnSlpMkCreatDescPool(
				break
				, (v_mk).m_vkstack.m_creatdescpool.m_creatinfo /** v_vkcreatinfo */
				, (v_mk).m_vkstack.m_creatdescpool.m_sz

				, (v_mk).m_union.m_alter.m_ptr->m_vkdescpool[
				ae2fVK_eAnnSlpDescPools_kTrain
				] /** r_vkdescpool */
				, (v_mk).m_reterr
				, (v_mk).m_union.m_alter.m_ptr->m_vkres

				, (vkdev) /** i_vkdev */ 
				, (vkalloccalls) /** i_vkptrAlloc */
				, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT /** i_vkdescpoolcreatflags */
				, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER /** i_vkdesctype */
				, 2
				);
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

/** 
 * @param r_handle must be destroyed by free().
 * @param i_second_raw must be guaranteed to be a raw stirng. 
 * */
ae2f_MAC() _ae2fVK_AnnSlpMkCLSPVVerbose_imp(
		void* restrict		r_handle,
		ae2f_err_t		ir_err,

		const char* restrict	i_first,
		const char* restrict	i_second,
		const char* restrict	i_third
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
		ae2f_float_t** restrict const data
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
						, ae2f_reinterpret_cast(void** restrict, data)
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

ae2f_MAC() _ae2fVK_AnnSlpMapRanged_imp(
		ae2fVK_AnnSlpMap_t		v_map,
		ae2f_err_t			r_err,
		ae2fVK_AnnSlp			slp,
		const VkDeviceSize		i_off,
		const VkDeviceSize		i_sz
		)
{
	__ae2fVK_AnnSlpMapRangedGeneric_imp(
			ae2f_float_t
			, (v_map).m_vkmmemr
			, slp
			, r_err
			, (v_map).m_map.m_f
			, i_off
			, i_sz
			);
}

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

#if !__ae2f_MACRO_GENERATED
#define COMMANDONRECORDING
#include <stdio.h>
#endif

/**
 * @param COMMANDONRECORDING 
 * You will need to push constant and dispatch later on there.
 * */
ae2f_MAC(COMMANDONRECORDING, ) _ae2fVK_AnnSlpGetCmd_imp(
		ae2fVK_AnnSlpGetCmd_t			v_getcmd,

		ae2fVK_AnnSlpPredictCmd_t		r_cmd,

		const VkCommandBuffer			i_vkcmdbuf,
		const ae2f_float_t*			i_inp,
		const VkCommandBufferUsageFlagBits	i_vkcmdbufuseflags,

		ae2fVK_AnnSlp		iv_slp,
		ae2f_err_t		iv_err,

		const uint32_t		i_desccount,

		const VkDeviceSize	i_offglob,
		const VkDeviceSize	i_szglob,

		const VkDeviceSize	i_offloc,
		const VkDeviceSize	i_szloc,

		const ae2fVK_eAnnSlpDescPools	i_descpool,
		const ae2fVK_eAnnSlpDescLayouts	i_desclayout,
		const ae2fVK_eAnnSlpPipes	i_pipe,
		const ae2fVK_eAnnSlpPipeLayouts	i_pipelayout
		)
{
	assert((iv_slp).m_vkres == VK_SUCCESS && "(iv_slp)'s state is bad.");
	assert((iv_err) == ae2f_errGlob_OK && "r_err's state is bad.");


	(v_getcmd).m_u0.m_vkdescsetallocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

	(v_getcmd).m_u0.m_vkdescsetallocinfo.descriptorPool
		= (iv_slp).m_vkdescpool[i_descpool];

	(v_getcmd).m_u0.m_vkdescsetallocinfo.descriptorSetCount
		= 1;

	(v_getcmd).m_u0.m_vkdescsetallocinfo.pSetLayouts 
		= &(iv_slp).m_vkdescsetlayout[i_desclayout];

	(v_getcmd).m_u0.m_vkdescsetallocinfo.pNext = NULL;

	assert((iv_slp).m_vkdev);
	assert((v_getcmd).m_u0.m_vkdescsetallocinfo.pSetLayouts);
	assert((v_getcmd).m_u0.m_vkdescsetallocinfo.pSetLayouts[i_desclayout]);
	assert((v_getcmd).m_u0.m_vkdescsetallocinfo.descriptorPool);

	do {
		if (((iv_slp).m_vkres = vkAllocateDescriptorSets(
						(iv_slp).m_vkdev
						, &(v_getcmd).m_u0.m_vkdescsetallocinfo
						, &(r_cmd).m_lpvkdescset
						)) != VK_SUCCESS)
		{
			printf("vkres: %d\n", (iv_slp).m_vkres);
			printf("HELKJDFSLKJSDF\n");
			assert((r_cmd).m_lpvkdescset);
			assert(!"vkAllocateDescriptorSets has failed.");
			break;
		}

		unless((r_cmd).m_lpvkdescset) {
			assert(!"vkAllocateDescriptorSets went NULL.");
			(iv_err) |= ae2f_errGlob_PTR_IS_NULL;
			break;
		}

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf[0]
			.buffer = (iv_slp).m_vkglobbuf;

		/** Offset: is not required. */
		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf[0]
			.offset = i_offglob;

		/** Range: Input Output Weight Bias */
		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf[0]
			.range = i_szglob;

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf[1]
			.buffer = (iv_slp).m_vklocbuf;

		/** Offset: is not required. */
		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf[1]
			.offset = i_offloc;

		/** Range: Input Output Weight Bias */
		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf[1]
			.range = i_szloc;


		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.dstSet = (r_cmd).m_lpvkdescset;

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.dstBinding = 0;
		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.dstArrayElement = 0;

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.descriptorCount = (i_desccount);

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
			.pBufferInfo = (v_getcmd).m_u0.m_vkdescwrdescinfo.m_buf;

		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset.pNext = NULL;
		(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset.pImageInfo = NULL;

		vkUpdateDescriptorSets(
				iv_slp.m_vkdev
				, 1
				, &(v_getcmd).m_u0.m_vkdescwrdescinfo.m_wrset
				, 0
				, NULL
				);

		(v_getcmd).m_u0.m_vkcmdbufbeginfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		(v_getcmd).m_u0.m_vkcmdbufbeginfo.pInheritanceInfo = NULL;
		(v_getcmd).m_u0.m_vkcmdbufbeginfo.flags = (i_vkcmdbufuseflags);
		(v_getcmd).m_u0.m_vkcmdbufbeginfo.pNext = NULL;

		if(((iv_slp).m_vkres = vkBeginCommandBuffer(
						i_vkcmdbuf
						, &(v_getcmd).m_u0.m_vkcmdbufbeginfo
						)) != VK_SUCCESS)
		{
			assert(!"vkBeginCommandBuffer has failed.");
			break;
		}

		vkCmdBindPipeline(
				i_vkcmdbuf
				, VK_PIPELINE_BIND_POINT_COMPUTE
				, (iv_slp).m_vkpipeline[i_pipe]
				);

		vkCmdBindDescriptorSets(
				i_vkcmdbuf
				, VK_PIPELINE_BIND_POINT_COMPUTE
				, (iv_slp).m_vkpipelayout[i_pipelayout]
				, 0
				, 1
				, &(r_cmd).m_lpvkdescset
				, 0
				, NULL
				);

		{ COMMANDONRECORDING }

		if(((iv_slp).m_vkres = vkEndCommandBuffer(i_vkcmdbuf)) != VK_SUCCESS) {
			assert(!"vkEndCommandBuffer has failed.");
			break;
		}
	} while(0);

	if(((iv_slp).m_vkres != VK_SUCCESS))
		(iv_err) |= ae2f_errGlob_NFOUND;
}

#undef COMMANDONRECORDING

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

		if((block).m_vkdescpool[0])
			vkDestroyDescriptorPool(
					(block).m_vkdev
					, (block).m_vkdescpool[0]
					, (block).m_vkalloccalls
					);

		if((block).m_vkdescpool[1])
			vkDestroyDescriptorPool(
					(block).m_vkdev
					, (block).m_vkdescpool[1]
					, (block).m_vkalloccalls
					);
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
			, 0
			, sizeof(ae2f_float_t) * ((vi_slp).m_slp.m_Slp[0].m_outc * ((vi_slp).m_slp.m_Slp[0].m_inc + 1))
			);
}

/** @brief Maps Weight and Bias */
ae2f_MAC() _ae2fVK_AnnSlpWBMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_float_t** restrict const ir_ptrweight
		, ae2f_float_t** restrict const ir_ptrbias
		)
{
	ae2f_err_t			v_err = 0;
	ae2fVK_AnnSlpMap_t	v_map;

	if(slp) {
		__ae2fVK_AnnSlpMapRanged_imp(
				v_map
				, v_err
				, slp[0]
				, 0
				, sizeof(ae2f_float_t) * ((slp)->m_slp.m_Slp[0].m_outc * ((slp)->m_slp.m_Slp[0].m_inc + 1))
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(ae2f_errGlob_OK == v_err) {
		if(ir_ptrweight) {
			*(ir_ptrweight) = (v_map).m_map.m_f;
		}

		if(ir_ptrbias) {
			*(ir_ptrbias) = 
				(v_map).m_map.m_f 
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
			, sizeof(ae2f_float_t) * ((vi_slp).m_slp.m_Slp[0].m_outc * (vi_slp).m_slp.m_Slp[0].m_inc + 1)
			, sizeof(ae2f_float_t) * ((vi_slp).m_slp.m_Slp[0].m_outc + (vi_slp).m_slp.m_Slp[0].m_inc)
			);
}



ae2f_MAC() _ae2fVK_AnnSlpIOMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_opt ae2f_float_t** restrict const ir_ptrinp
		, ae2f_opt ae2f_float_t** restrict const ir_ptrout
		)
{
	ae2f_err_t		v_err = 0;
	ae2fVK_AnnSlpMap_t	v_map;

	if(slp) {
		__ae2fVK_AnnSlpMapRanged_imp(
				v_map
				, v_err
				, slp[0]
				, sizeof(ae2f_float_t) * 
				((slp)->m_slp.m_Slp[0].m_outc * ((slp)->m_slp.m_Slp[0].m_inc + 1))
				, sizeof(ae2f_float_t) * ((slp)->m_slp.m_Slp[0].m_outc + (slp)->m_slp.m_Slp[0].m_inc)
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(ae2f_errGlob_OK == v_err) {
		if(ir_ptrinp) {
			*(ir_ptrinp) = (v_map).m_map.m_f;
		}

		if(ir_ptrout) {
			*(ir_ptrout) = (v_map).m_map.m_f + (slp)->m_slp.m_Slp[0].m_inc;
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
			, sizeof(ae2f_float_t) * 
			((vi_slp).m_slp.m_Slp[0].m_outc * ((vi_slp).m_slp.m_Slp[0].m_inc + 2) + (vi_slp).m_slp.m_Slp[0].m_inc)
			, sizeof(ae2f_float_t) * ((vi_slp).m_slp.m_Slp[0].m_outc)
			);
}

ae2f_MAC() _ae2fVK_AnnSlpDeltaMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_opt ae2f_float_t** restrict const ir_ptrdelta
		)
{
	ae2f_err_t		v_err = 0;
	ae2fVK_AnnSlpMap_t	v_map;

	if((slp) && (ir_ptrdelta)) {
		__ae2fVK_AnnSlpMapRanged_imp(
				v_map
				, v_err
				, slp[0]
				, sizeof(ae2f_float_t) * 
				((slp)->m_slp.m_Slp[0].m_outc * ((slp)->m_slp.m_Slp[0].m_inc + 2) + (slp)->m_slp.m_Slp[0].m_inc)
				, sizeof(ae2f_float_t) * ((slp)->m_slp.m_Slp[0].m_outc)
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(ae2f_errGlob_OK == v_err) {
		if(ir_ptrdelta) {
			*(ir_ptrdelta) = (v_map).m_map.m_f;
		}
	}

	if(r_err) {
		*(r_err) |= (v_err);
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
			, sizeof(ae2f_float_t) * 
			((vi_slp).m_slp.m_Slp[0].m_outc * ((vi_slp).m_slp.m_Slp[0].m_inc + 3) + (vi_slp).m_slp.m_Slp[0].m_inc)
			, sizeof(ae2f_float_t) * ((vi_slp).m_slp.m_Slp[0].m_outc)
			);
}


ae2f_MAC() _ae2fVK_AnnSlpGoalMap(
		ae2f_err_t*			r_err,
		ae2fVK_AnnSlp*			slp
		, ae2f_opt ae2f_float_t** restrict const ir_ptrgoal
		)
{
	ae2f_err_t		v_err = 0;
	ae2fVK_AnnSlpMap_t	v_map;

	if((slp) && (ir_ptrgoal)) {
		__ae2fVK_AnnSlpMapRanged_imp(
				v_map
				, v_err
				, slp[0]
				, sizeof(ae2f_float_t) * 
				((slp)->m_slp.m_Slp[0].m_outc * ((slp)->m_slp.m_Slp[0].m_inc + 3) + (slp)->m_slp.m_Slp[0].m_inc)
				, sizeof(ae2f_float_t) * ((slp)->m_slp.m_Slp[0].m_outc)
				);
	} else {
		assert(!"Got null pointer from slp");
		v_err |= ae2f_errGlob_PTR_IS_NULL;
	}

	if(ae2f_errGlob_OK == v_err) {
		if(ir_ptrgoal) {
			*(ir_ptrgoal) = (v_map).m_map.m_f;
		}
	}

	if(r_err) {
		*(r_err) |= (v_err);
	}
}


ae2f_MAC() _ae2fVK_AnnSlpPredictPerformed_imp(
		ae2fVK_AnnSlpGetCmd_t			v_predict,

		ae2fVK_AnnSlpPredictCmd_t		r_cmd,

		const VkCommandBuffer			i_vkcmdbuf,
		const ae2f_float_t*			i_inp,
		const VkCommandBufferUsageFlagBits	i_vkcmdbufuseflags,

		ae2fVK_AnnSlp				iv_slp,
		ae2f_err_t				iv_err
		)
{	
	__ae2fVK_AnnSlpGetCmd_imp(
			ae2f_CastMerge(
				{
				vkCmdPushConstants(
						i_vkcmdbuf
						, iv_slp.m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kPredict]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, 0
						, sizeof(uint32_t)
						, &(iv_slp).m_slp.m_Slp[0].m_inc
						);

				vkCmdDispatch(
						i_vkcmdbuf
						, (iv_slp).m_slp.m_Slp[0].m_outc
						, (iv_slp).m_slp.m_Slp[0].m_inc
						, 1
					     );
				}
				)
			, v_predict
			, r_cmd
			, i_vkcmdbuf
			, i_inp
			, i_vkcmdbufuseflags
			, iv_slp
			, iv_err

			, 1

			, 0
			, sizeof(ae2f_float_t) 
			* ((iv_slp).m_slp.m_Slp[0].m_outc * ((iv_slp).m_slp.m_Slp[0].m_inc + 2)
					+ (iv_slp).m_slp.m_Slp[0].m_inc
			  )

			, 0, sizeof(ae2f_float_t)

			, ae2fVK_eAnnSlpDescPools_kPredict /** i_descpool */
			, ae2fVK_eAnnSlpDescLayouts_kPredict /** i_desclayout */
			, ae2fVK_eAnnSlpPipes_kPredict /** i_pipe */
			, ae2fVK_eAnnSlpPipeLayouts_kPredict /** i_pipelayout */
			);
}

ae2f_MAC() _ae2fVK_AnnSlpPredictFree_imp(
		ae2fVK_AnnSlp iv_slp,
		ae2fVK_AnnSlpPredictCmd_t iv_cmd
		)
{
	if(((iv_slp).m_vkres = vkFreeDescriptorSets(
					(iv_slp).m_vkdev
					, (iv_slp).m_vkdescpool[ae2fVK_eAnnSlpDescPools_kPredict]
					, 1
					, &(iv_cmd).m_lpvkdescset
					)) != VK_SUCCESS)
		assert(!"vkFreeDescriptorSets has failed.");
}

ae2f_MAC() _ae2fVK_AnnSlpTrainFree_imp(
		ae2fVK_AnnSlp iv_slp,
		ae2fVK_AnnSlpPredictCmd_t iv_cmd
		)
{
	if(((iv_slp).m_vkres = vkFreeDescriptorSets(
					(iv_slp).m_vkdev
					, (iv_slp).m_vkdescpool[ae2fVK_eAnnSlpDescPools_kTrain]
					, 1
					, &(iv_cmd).m_lpvkdescset
					)) != VK_SUCCESS)
		assert(!"vkFreeDescriptorSets has failed.");
}

ae2f_MAC() _ae2fVK_AnnSlpTrainPerformed_imp(
		ae2fVK_AnnSlpGetCmd_t			v_predict,

		ae2fVK_AnnSlpPredictCmd_t		r_cmd,

		const VkCommandBuffer			i_vkcmdbuf,
		const ae2f_float_t*			i_inp,
		const VkCommandBufferUsageFlagBits	i_vkcmdbufuseflags,

		ae2fVK_AnnSlp				iv_slp,
		ae2f_err_t				iv_err
		)
{
	__ae2fVK_AnnSlpGetCmd_imp(
			ae2f_CastMerge(
				{
				vkCmdPushConstants(
						i_vkcmdbuf
						, iv_slp.m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, 0
						, sizeof(ae2f_float_t)
						, &(iv_slp).m_slp.m_learningrate
						);

				vkCmdPushConstants(
						i_vkcmdbuf
						, iv_slp.m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, sizeof(ae2f_float_t)
						, sizeof(ae2f_float_t)
						, &(iv_slp).m_slp.m_learningrate_bias
						);

				vkCmdDispatch(
						i_vkcmdbuf
						, (iv_slp).m_slp.m_Slp[0].m_outc
						, (iv_slp).m_slp.m_Slp[0].m_inc
						, 1
					     );
				}
	)
		, v_predict
		, r_cmd
		, i_vkcmdbuf
		, i_inp
		, i_vkcmdbufuseflags
		, iv_slp
		, iv_err

		, 2 

		, 0
		, sizeof(ae2f_float_t) 
		* ((iv_slp).m_slp.m_Slp[0].m_outc * ((iv_slp).m_slp.m_Slp[0].m_inc + 4)
				+ (iv_slp).m_slp.m_Slp[0].m_inc)

		, 0
		, sizeof(ae2f_float_t) * ((iv_slp).m_slp.m_Slp[0].m_outc)

		, ae2fVK_eAnnSlpDescPools_kTrain /** i_descpool */
		, ae2fVK_eAnnSlpDescLayouts_kTrain /** i_desclayout */
		, ae2fVK_eAnnSlpPipes_kTrain /** i_pipe */
		, ae2fVK_eAnnSlpPipeLayouts_kTrain /** i_pipelayout */
		);
}

#endif
