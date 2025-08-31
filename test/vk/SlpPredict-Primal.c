#include <ae2fVK/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include "../vk.h"
#include "ae2f/Cast.h"
#include "ae2f/Float.h"
#include "ae2f/errGlob.h"

#include <string.h>
#include <math.h>


#if 1

ae2fVK_AnnSlpMk_t	mk;

size_t			mapinp;

static void
Act(ae2f_float_t* r, ae2f_float_t x) {
	*r = 1.0 / (1.0 + exp(-x));
}

static void
ActDeriv(ae2f_float_t* r, ae2f_float_t output) {
	output += 1e-7;
	*r = output * (1.0 - output);
}

/** Cross entrophy */
static void
LossDeriv(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
	ae2f_float_t epsilon = 1e-7; // Small value to prevent division by zero
	ae2f_float_t o_i = output[i];
	// Clip output to avoid log(0) or division by zero
	o_i = o_i < epsilon ? epsilon : (o_i > 1.0 - epsilon ? 1.0 - epsilon : o_i);
	*r = (o_i - target[i]) / (c * o_i * (1.0 - o_i));
}

ae2f_float_t*	map;

#define fgetc(...)

int main() {
	Test_VkInit();
	memset(&(mk), 0, sizeof(mk));

	__ae2fVK_AnnSlpMk_imp(
			mk,
			0, 0, 0
			, 2, 1, 0, 0
			, Act, ActDeriv, LossDeriv
			, 0, 0
			, vkdev
			, vkphydevmemprops
			, NULL
			, 

			"#define LOSS_DERIV(r, y, y_desired, i, c)  \n"
			"#define ACT(r, x) { (*(r) = (1.0 / (1.0 + exp(-x)))); } \n"
			, "/** This is also a comment */\n"
			);

	puts("__ae2fVK_AnnSlpMk_imp is done");
	fgetc(stdin);

	assert(mk.m_U0.m_alter.m_ptr->m_slp.m_Slp[0].m_inc == 2);
	assert(mk.m_U0.m_alter.m_ptr->m_slp.m_Slp[0].m_outc == 1);

	printf("Stateval: %d\n", (mk).m_U0.m_alter.m_ptr->m_vkres);

	assert(mk.m_U0.m_alter.m_ptr && "__ae2fVK_AnnSlpMk_imp has failed");
	assert(mk.m_reterr == ae2f_errGlob_OK);

	assert(mk.m_U0.m_alter.m_ptr->m_vkres == VK_SUCCESS);

	{
		ae2f_float_t*	weight;
		ae2f_float_t*	bias;
		ae2f_err_t	e;
		size_t 		i, j;

		ae2fVK_AnnSlpUnMap_t	v_unmap;

		__ae2fVK_AnnSlpWBMap(
				&e
				, mk.m_U0.m_alter.m_ptr
				, &weight
				, &bias
				);

		assert(ae2f_errGlob_OK == e && "__ae2fVK_AnnSlpWBMap has failed.");
		assert(weight);
		assert(bias);

		for(i = 0; i < 2; i++) {
			bias[0] = ((double)rand() / RAND_MAX) -  0.5;
			weight[i] = ((double)rand() / RAND_MAX) - 0.5;
		}

		__ae2fVK_AnnSlpWBUnMap_imp(
				v_unmap
				, mk.m_U0.m_alter.m_ptr[0]
				);
	}

	{
		ae2f_err_t err = 0;
		ae2fVK_AnnSlpUnMap_t v_unmap;

		ae2f_float_t
			*	OutputMapped = NULL,
			*	InputMapped = NULL
				;

		__ae2fVK_AnnSlpIOMap(
				&err
				, mk.m_U0.m_alter.m_ptr
				, (&InputMapped)
				, (&OutputMapped)
				);

		puts("Before OutputMapped written");
		fgetc(stdin);

		assert(OutputMapped);
		assert(InputMapped);

		assert((mk.m_U0.m_alter.m_ptr[0].m_vkres) == VK_SUCCESS);

		OutputMapped[0] = 7;

		InputMapped[0] = 7;
		InputMapped[1] = 7;

		printf("Inputmapped before predict: %f %f\n", InputMapped[0], InputMapped[1]);
		printf("Outputmapped before predict: %f\n", OutputMapped[0]);

		fgetc(stdin);
		__ae2fVK_AnnSlpIOUnMap_imp(v_unmap, mk.m_U0.m_alter.m_ptr[0]);

		puts("After __ae2fVK_AnnSlpIOUnMapOutput_imp.");
		fgetc(stdin);
	}

	VkCommandBuffer	vkcmdbuf = NULL;
	VkCommandPool	vkcmdpool = NULL;

	{
		VkCommandPoolCreateInfo	vkcmdpoolcreatinfo;
		vkcmdpoolcreatinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkcmdpoolcreatinfo.pNext = NULL;
		vkcmdpoolcreatinfo.queueFamilyIndex = 0;
		vkcmdpoolcreatinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		if((((mk).m_U0.m_alter.m_ptr->m_vkres) = vkCreateCommandPool(
						vkdev
						, &vkcmdpoolcreatinfo
						, NULL
						, &vkcmdpool
						)) != VK_SUCCESS)
		{
			printf("Errval: %d\n", (mk).m_U0.m_alter.m_ptr->m_vkres);
			assert(!"vkCreateCommandPool has failed.");
			return -1;
		}

		puts("After vkCreateCommandPool.");
		fgetc(stdin);

		unless(vkcmdpool) {
			assert(!"vkCreateCommandPool went null");
			return -1;
		}
	}

	{
		VkCommandBufferAllocateInfo vkcmdallocinfo;
		vkcmdallocinfo.commandBufferCount = 1;
		vkcmdallocinfo.pNext = NULL;
		vkcmdallocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		vkcmdallocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		vkcmdallocinfo.commandPool = vkcmdpool;

		if((mk.m_U0.m_alter.m_ptr->m_vkres = 
					vkAllocateCommandBuffers(
						(vkdev)
						, &vkcmdallocinfo
						, &vkcmdbuf
						)) != VK_SUCCESS)
		{
			printf("Errval: %d\n", (mk).m_U0.m_alter.m_ptr->m_vkres);
			assert(!"vkAllocateCommandBuffers has failed.");
			return -1;
		}

		puts("After vkAllocateCommandBuffers.");
		fgetc(stdin);

		unless((vkcmdbuf))
		{
			printf("Errval: %d\n", (mk).m_U0.m_alter.m_ptr->m_vkres);
			assert(!"vkAllocateCommandBuffers went null.");
			return -1;
		}
	}

	{
		ae2fVK_AnnSlpCreatDescPool_t	v_poolmk;
		ae2fVK_AnnSlpDescPoolCmdMk_t	v_cmdmk;

		ae2fVK_AnnSlpDescPool		v_descpool;
		ae2fVK_AnnSlpDescPoolCmd	v_descpoolcmd;


		ae2f_err_t err = 0;

		puts("Before __ae2fVK_AnnSlpPredictPerformed_imp");

		/** Getting pool */
		__ae2fVK_AnnSlpDescPoolMk_imp(
				v_poolmk
				, v_descpool
				, err
				, (mk.m_U0.m_alter.m_ptr[0])
				, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
				, 1
				);

		/** Getting predict command */
		__ae2fVK_AnnSlpDescPoolCmdMkPredict_imp(
				v_cmdmk
				, (mk.m_U0.m_alter.m_ptr[0])
				, err
				, v_descpool
				, v_descpoolcmd
				, vkcmdbuf
				);

		puts("After __ae2fVK_AnnSlpPredictPerformed_imp");
		fgetc(stdin);

		if(err != ae2f_errGlob_OK) {
			if (err & ae2f_errGlob_NFOUND)
				printf(
						"Vulkan state number %d: \n"
						, mk.m_U0.m_alter.m_ptr->m_vkres
				      );

			printf("Error value %x, \n", err);
			assert(!"__ae2fVK_AnnSlpPredictPerformed_imp has failed.");
			return -1;
		}

		unless((v_descpoolcmd).m_vkdescset) {
			assert(!"__ae2fVK_AnnSlpPredictPerformed_imp went null.");
			return -1;
		}

		{
			VkSubmitInfo submit_info = {
				.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
				.commandBufferCount = 1,
				.pCommandBuffers = &vkcmdbuf,
			};

			ae2fVK_AnnSlpUnMap_t	v_unmap;

			ae2f_float_t
				*	OutputMapped = NULL,
				*	InputMapped = NULL
					;

			if (vkQueueSubmit(
						vkqueue
						, 1
						, &submit_info
						, VK_NULL_HANDLE
					 ) != VK_SUCCESS)
			{
				assert(!"vkQueueSubmit failed");
				return -1;
			}

			if (vkQueueWaitIdle(vkqueue) != VK_SUCCESS) {
				assert(!"vkQueueWaitIdle failed");
				return -1;
			}

			puts("After vkQueueSubmit");
			fgetc(stdin);

			if (vkQueueSubmit(
						vkqueue
						, 1
						, &submit_info
						, VK_NULL_HANDLE
					 ) != VK_SUCCESS)
			{
				assert(!"vkQueueSubmit failed");
				return -1;
			}

			if (vkQueueWaitIdle(vkqueue) != VK_SUCCESS) {
				assert(!"vkQueueWaitIdle failed");
				return -1;
			}

			puts("After vkQueueSubmit 2");
			fgetc(stdin);

			if (vkQueueSubmit(
						vkqueue
						, 1
						, &submit_info
						, VK_NULL_HANDLE
					 ) != VK_SUCCESS)
			{
				assert(!"vkQueueSubmit failed");
				return -1;
			}

			if (vkQueueWaitIdle(vkqueue) != VK_SUCCESS) {
				assert(!"vkQueueWaitIdle failed");
				return -1;
			}

			puts("After vkQueueSubmit 3");
			fgetc(stdin);

			puts("After vkQueueWaitIdle");
			fgetc(stdin);

			/** Check is required, but not planned to write for no */
			__ae2fVK_AnnSlpIOMap(
					&err
					, mk.m_U0.m_alter.m_ptr
					, &InputMapped
					, &OutputMapped
					);

			assert(OutputMapped);
			assert((mk.m_U0.m_alter.m_ptr[0].m_vkres) == VK_SUCCESS);

			printf("Inputmapped before predict: %f %f\n", InputMapped[0], InputMapped[1]);
			printf("Outputmapped after prediction %f\n", OutputMapped[0]);

			__ae2fVK_AnnSlpIOUnMap_imp(
					v_unmap
					, *mk.m_U0.m_alter.m_ptr
					);
		}

		__ae2fVK_AnnSlpDescPoolCmdClean_imp(
				mk.m_U0.m_alter.m_ptr[0]
				, v_descpool
				, v_descpoolcmd
				);

		__ae2fVK_AnnSlpDescPoolClean_imp(
				mk.m_U0.m_alter.m_ptr[0]
				, v_descpool
				);
	}

	__ae2fVK_AnnSlpClean_imp((*mk.m_U0.m_alter.m_ptr));
	vkFreeCommandBuffers(vkdev, vkcmdpool, 1, &vkcmdbuf);
	vkDestroyCommandPool(vkdev, vkcmdpool, NULL);

	memset(mk.m_U0.m_alter.m_ptr, 0, sizeof(*mk.m_U0.m_alter.m_ptr));
	free(mk.m_U0.m_alter.m_ptr);

	memset(&mk, 0, sizeof(mk));
	Test_VkEnd();
	return 0;
}

#else

int main() {
	return 0;
}

#endif
