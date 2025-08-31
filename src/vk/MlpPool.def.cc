#ifndef ae2fVK_AnnMlpPool_h

#if __ae2f_MACRO_GENERATED
#define ae2fVK_AnnMlpPool_h
#endif

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#include <assert.h>
#endif

#include <ae2fVK/Ann/Mlp.h>
#include <ae2fVK/Ann/MlpPool.auto.h>
#include <ae2fVK/Ann/SlpPool.auto.h>

ae2f_MAC() _ae2fVK_AnnMlpDescPoolCmdMkPredict_imp(
		ae2fVK_AnnMlpDescPoolCmdMk_t	v_cmdmk,

		ae2fVK_AnnMlp			iv_mlp,
		ae2f_err_t			iv_err,
		const ae2fVK_AnnSlpDescPool	i_pool,

		ae2fVK_AnnMlpDescPoolCmd	r_cmd,

		const VkCommandBuffer		i_vkcmdbuf
		)
{
	__ae2fVK_AnnMlpDescPoolCmdMk_imp(
			ae2f_CastMerge(
				{
				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_mlp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kPredict]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, 0
						, sizeof(uint32_t)
						, &(iv_mlp).m_mlp.m_depth
						);

				vkCmdDispatch(
						i_vkcmdbuf
						, (iv_mlp).m_mlp.m_outc
						, (iv_mlp).m_mlp.m_outc
						, 1
					     );
				}
				)

			, v_cmdmk
			, r_cmd
			, iv_mlp
			, i_pool
			, iv_err

			, 1

			, 0
			, 0
			+ sizeof(uint32_t) * (iv_mlp).m_mlp.m_depth
			+ sizeof(ae2f_float_t) * ((iv_mlp).m_mlp.m_outc) * ((iv_mlp).m_mlp.m_depth * 2 + 1)
			+ sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * (iv_mlp).m_mlp.m_outc * (iv_mlp).m_mlp.m_depth
			, 0, sizeof(ae2f_float_t) * (iv_mlp).m_mlp.m_outc * 2

			, ae2fVK_eAnnMlpDescLayouts_ONLY /** i_desclayout */
			, ae2fVK_eAnnMlpPipes_kPredict /** i_pipe */
			, ae2fVK_eAnnMlpPipeLayouts_kPredict /** i_pipelayout */
			, i_vkcmdbuf
			);
}

ae2f_MAC() _ae2fVK_AnnMlpDescPoolCmdMkTrain_imp(
		ae2fVK_AnnMlpDescPoolCmdMk_t	v_cmdmk,

		ae2fVK_AnnMlp			iv_mlp,
		ae2f_err_t			iv_err,
		const ae2fVK_AnnMlpDescPool	i_pool,

		ae2fVK_AnnSlpDescPoolCmd	r_cmd,

		const VkCommandBuffer		i_vkcmdbuf

		)
{
	__ae2fVK_AnnSlpDescPoolCmdMk_imp(
			ae2f_CastMerge(
				{
				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_mlp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, 0
						, sizeof(uint32_t)
						, &(iv_mlp).m_mlp.m_depth
						);

				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_mlp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, ae2f_CmpGetGt(sizeof(ae2f_float_t), sizeof(uint32_t))
						, sizeof(ae2f_float_t)
						, &(iv_mlp).m_mlp.m_learningrate
						);

				vkCmdPushConstants(
						i_vkcmdbuf
						, (iv_mlp).m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_kTrain]
						, VK_SHADER_STAGE_COMPUTE_BIT
						, ae2f_CmpGetGt(sizeof(ae2f_float_t), sizeof(uint32_t)) * 2
						, sizeof(ae2f_float_t)
						, &(iv_mlp).m_mlp.m_learningrate_bias
						);

				vkCmdDispatch(
						i_vkcmdbuf
						, (iv_mlp).m_mlp.m_outc
						, (iv_mlp).m_mlp.m_outc
						, 1
					     );
				}
	)
		, v_cmdmk
		, r_cmd
		, iv_mlp
		, i_pool
		, iv_err

		, 2

		, 0
		, __ae2fVK_AnnMlpGlobMemSz((iv_mlp).m_mlp.m_depth, (iv_mlp).m_mlp.m_outc)

		, 0, (((iv_mlp).m_mlp.m_outc * ((iv_mlp).m_mlp.m_depth + 2)) * sizeof(ae2f_float_t))

		, ae2fVK_eAnnMlpDescLayouts_ONLY /** i_desclayout */
		, ae2fVK_eAnnMlpPipes_kTrainAuto /** i_pipe */
		, ae2fVK_eAnnMlpPipeLayouts_kTrain /** i_pipelayout */
		, i_vkcmdbuf
		);
}

#endif
