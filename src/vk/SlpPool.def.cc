#ifndef ae2fVK_AnnSlpPool_h

#if __ae2f_MACRO_GENERATED
#define ae2fVK_AnnSlpPool_h
#endif

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#include <assert.h>
#endif

#include <ae2fVK/Ann/Slp.h>
#include <ae2fVK/Ann/SlpPool.auto.h>

/**
 * @param 
 * i_vkmaxdescsets Note that would be the max possible amount for 
 * making ae2fVK_AnnSlpDescPoolCmd.
 *
 * */
ae2f_MAC() _ae2fVK_AnnSlpDescPoolMkGeneric_imp(
		ae2fVK_AnnSlpCreatDescPool_t	v_descpool,
		ae2fVK_AnnSlpDescPool	r_pool,
		ae2f_err_t		r_reterr,

		ae2fVK_AnnSlp		iv_slp,

		const VkDescriptorPoolCreateFlagBits	i_vkdescpoolcreatflags,
		const VkDescriptorType			i_vkdesctype,
		const uint32_t				i_vkdescpoolsz_count,
		const uint32_t				i_vkmaxdescsets
		)
{
	(r_pool).m_vkdescpool = 0;
	assert(r_reterr == ae2f_errGlob_OK);

	__ae2fVK_AnnSlpCreatDescPoolVerbose_imp(
			  (v_descpool).m_U0.m_vkdescpoolcreatinfo
			, (v_descpool).m_U0.m_vkdescpoolsz
			, (r_pool).m_vkdescpool
			, r_reterr
			, (iv_slp).m_vkres
			, (iv_slp).m_vkdev
			, (iv_slp).m_vkalloccalls
			, i_vkdescpoolcreatflags
			, i_vkdesctype
			, i_vkdescpoolsz_count
			, i_vkmaxdescsets
			);

	if((iv_slp).m_vkres != VK_SUCCESS) {
		(r_reterr) |= ae2f_errGlob_NFOUND;
		assert(!"__ae2fVK_AnnSlpCreatDescPoolVerbose_imp has failed (Vulkan)");
	}

	if((r_reterr))
		assert(!"__ae2fVK_AnnSlpCreatDescPoolVerbose_imp has failed.");

	unless((r_pool).m_vkdescpool) {
		assert(!"__ae2fVK_AnnSlpCreatDescPoolVerbose_imp went null.");
		(r_reterr) |= ae2f_errGlob_ALLOC_FAILED;
	}
}

ae2f_MAC() _ae2fVK_AnnSlpDescPoolMk_imp(
		ae2fVK_AnnSlpCreatDescPool_t	v_descpool,
		ae2fVK_AnnSlpDescPool	r_pool,
		ae2f_err_t		r_reterr,

		ae2fVK_AnnSlp		iv_slp,

		const VkDescriptorPoolCreateFlagBits	i_vkdescpoolcreatflags,
		const uint32_t				i_vkmaxdescsets
		)
{
	__ae2fVK_AnnSlpDescPoolMkGeneric_imp(
			v_descpool
			, r_pool
			, r_reterr
			, iv_slp
			, i_vkdescpoolcreatflags
			, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
			, 2, i_vkmaxdescsets
			);
}


ae2f_MAC() _ae2fVK_AnnSlpDescPoolClean_imp(
		const ae2fVK_AnnSlp		i_slp,
		const ae2fVK_AnnSlpDescPool	i_pool
		)
{
	vkDestroyDescriptorPool(
			(i_slp).m_vkdev
			, (i_pool).m_vkdescpool
			, (i_slp).m_vkalloccalls
			);
}

#if !__ae2f_MACRO_GENERATED
#define COMMANDONRECORDING
#endif

/** 
 * @param  i_desccount Descriptor Count or Kernel parameter(argument) count.
 * */
ae2f_MAC(COMMANDONRECORDING,) _ae2fVK_AnnSlpDescPoolCmdMk_imp(
		ae2fVK_AnnSlpDescPoolCmdMk_t	v_cmdmk,

		ae2fVK_AnnSlpDescPoolCmd	r_cmd,

		ae2fVK_AnnSlp			iv_slp,
		const ae2fVK_AnnSlpDescPool	i_pool,

		ae2f_err_t		iv_err,

		const uint32_t		i_desccount,

		const VkDeviceSize	i_offglob,
		const VkDeviceSize	i_szglob,

		const VkDeviceSize	i_offloc,
		const VkDeviceSize	i_szloc,
		const ae2fVK_eAnnSlpDescLayouts	i_desclayout,
		const ae2fVK_eAnnSlpPipes	i_pipe,
		const ae2fVK_eAnnSlpPipeLayouts	i_pipelayout,


		const VkCommandBuffer			i_vkcmdbuf
		)
{
	assert((iv_slp).m_vkres == VK_SUCCESS && "(iv_slp)'s state is bad.");
	assert((iv_err) == ae2f_errGlob_OK && "r_err's state is bad.");


	(v_cmdmk).m_u0.m_vkdescsetallocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

	(v_cmdmk).m_u0.m_vkdescsetallocinfo.descriptorPool
		= (i_pool).m_vkdescpool;

	(v_cmdmk).m_u0.m_vkdescsetallocinfo.descriptorSetCount
		= 1;

	(v_cmdmk).m_u0.m_vkdescsetallocinfo.pSetLayouts 
		= &(iv_slp).m_vkdescsetlayout[i_desclayout];

	(v_cmdmk).m_u0.m_vkdescsetallocinfo.pNext = NULL;

	assert((iv_slp).m_vkdev);
	assert((v_cmdmk).m_u0.m_vkdescsetallocinfo.pSetLayouts);
	assert((v_cmdmk).m_u0.m_vkdescsetallocinfo.pSetLayouts[i_desclayout]);
	assert((v_cmdmk).m_u0.m_vkdescsetallocinfo.descriptorPool);

	do {
		if (((iv_slp).m_vkres = vkAllocateDescriptorSets(
						(iv_slp).m_vkdev
						, &(v_cmdmk).m_u0.m_vkdescsetallocinfo
						, &(r_cmd).m_vkdescset
						)) != VK_SUCCESS)
		{
			assert((r_cmd).m_vkdescset);
			assert(!"vkAllocateDescriptorSets has failed.");
			break;
		}

		unless((r_cmd).m_vkdescset) {
			assert(!"vkAllocateDescriptorSets went NULL.");
			(iv_err) |= ae2f_errGlob_PTR_IS_NULL;
			break;
		}

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf[0]
			.buffer = (iv_slp).m_vkglobbuf;

		/** Offset: is not required. */
		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf[0]
			.offset = i_offglob;

		/** Range: Input Output Weight Bias */
		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf[0]
			.range = i_szglob;

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf[1]
			.buffer = (iv_slp).m_vklocbuf;

		/** Offset: is not required. */
		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf[1]
			.offset = i_offloc;

		/** Range: Input Output Weight Bias */
		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf[1]
			.range = i_szloc;


		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.dstSet = (r_cmd).m_vkdescset;

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.dstBinding = 0;
		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.dstArrayElement = 0;

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.descriptorCount = (i_desccount);

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
			.pBufferInfo = (v_cmdmk).m_u0.m_vkdescwrdescinfo.m_buf;

		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset.pNext = NULL;
		(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset.pImageInfo = NULL;

		vkUpdateDescriptorSets(
				iv_slp.m_vkdev
				, 1
				, &(v_cmdmk).m_u0.m_vkdescwrdescinfo.m_wrset
				, 0
				, NULL
				);

		(v_cmdmk).m_u0.m_vkcmdbufbeginfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		(v_cmdmk).m_u0.m_vkcmdbufbeginfo.pInheritanceInfo = NULL;
		(v_cmdmk).m_u0.m_vkcmdbufbeginfo.flags = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		(v_cmdmk).m_u0.m_vkcmdbufbeginfo.pNext = NULL;

		if(((iv_slp).m_vkres = vkBeginCommandBuffer(
						i_vkcmdbuf
						, &(v_cmdmk).m_u0.m_vkcmdbufbeginfo
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
				, &(r_cmd).m_vkdescset
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
ae2f_MAC() _ae2fVK_AnnSlpDescPoolCmdClean_imp(
		ae2fVK_AnnSlp			iv_slp,
		const ae2fVK_AnnSlpDescPool	i_pool,
		const ae2fVK_AnnSlpDescPoolCmd	i_cmd
		)
{
	if(((iv_slp).m_vkres = vkFreeDescriptorSets(
					(iv_slp).m_vkdev
					, (i_pool).m_vkdescpool
					, 1
					, &(i_cmd).m_vkdescset
					)) != VK_SUCCESS)
		assert(!"vkFreeDescriptorSets has failed.");
}

ae2f_MAC() _ae2fVK_AnnSlpDescPoolCmdMkPredict_imp(
		ae2fVK_AnnSlpDescPoolCmdMk_t	v_cmdmk,

		ae2fVK_AnnSlp			iv_slp,
		ae2f_err_t			iv_err,
		const ae2fVK_AnnSlpDescPool	i_pool,

		ae2fVK_AnnSlpDescPoolCmd	r_cmd,

		const VkCommandBuffer		i_vkcmdbuf
		)
{
	__ae2fVK_AnnSlpDescPoolCmdMk_imp(
			ae2f_CastMerge(
				{
				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_slp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kPredict]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, 0
						, sizeof(uint32_t)
						, &(iv_slp).m_slp.m_Slp[0].m_inc
						);

				vkCmdDispatch(
						(i_vkcmdbuf)
						, (iv_slp).m_slp.m_Slp[0].m_outc
						, (iv_slp).m_slp.m_Slp[0].m_inc
						, 1
					     );
				}
				)
			, v_cmdmk
			, r_cmd
			, iv_slp
			, i_pool
			, iv_err
			, 1
			, 0 
			, __ae2fVK_AnnSlpOutOff((iv_slp).m_slp.m_Slp[0].m_inc, (iv_slp).m_slp.m_Slp[0].m_outc)
			+ __ae2fVK_AnnSlpOutSz((iv_slp).m_slp.m_Slp[0].m_inc, (iv_slp).m_slp.m_Slp[0].m_outc)

			, 0
			, 0

			, ae2fVK_eAnnSlpDescLayouts_kPredict /** i_desclayout */
			, ae2fVK_eAnnSlpPipes_kPredict /** i_pipe */
			, ae2fVK_eAnnSlpPipeLayouts_kPredict /** i_pipelayout */

			, i_vkcmdbuf
			);
}

ae2f_MAC() _ae2fVK_AnnSlpDescPoolCmdMkTrain_imp(
		ae2fVK_AnnSlpDescPoolCmdMk_t	v_cmdmk,

		ae2fVK_AnnSlp			iv_slp,
		ae2f_err_t			iv_err,
		const ae2fVK_AnnSlpDescPool	i_pool,

		ae2fVK_AnnSlpDescPoolCmd	r_cmd,

		const VkCommandBuffer		i_vkcmdbuf
		)
{
	__ae2fVK_AnnSlpDescPoolCmdMk_imp(
			ae2f_CastMerge(
				{
				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_slp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, 0
						, sizeof(ae2f_float_t)
						, &(iv_slp).m_slp.m_learningrate
						);

				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_slp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, sizeof(ae2f_float_t)
						, sizeof(ae2f_float_t)
						, &(iv_slp).m_slp.m_learningrate_bias
						);

				vkCmdDispatch(
						(i_vkcmdbuf)
						, (iv_slp).m_slp.m_Slp[0].m_outc
						, (iv_slp).m_slp.m_Slp[0].m_inc
						, 1
					     );
				}
	)
		, v_cmdmk
		, r_cmd
		, iv_slp
		, i_pool
		, iv_err
		, 1
		, 0 
		, __ae2fVK_AnnSlpGlobSz((iv_slp).m_slp.m_Slp[0].m_inc, (iv_slp).m_slp.m_Slp[0].m_outc)

		, 0
		, sizeof(ae2f_float_t) * ((iv_slp).m_slp.m_Slp[0].m_outc)

		, ae2fVK_eAnnSlpDescLayouts_kTrain /** i_desclayout */
		, ae2fVK_eAnnSlpPipes_kTrain /** i_pipe */
		, ae2fVK_eAnnSlpPipeLayouts_kTrain /** i_pipelayout */

		, i_vkcmdbuf
		);
}

#endif
