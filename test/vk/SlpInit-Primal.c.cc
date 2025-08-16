#include <ae2fVK/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include "../vk.h"
#include "ae2f/Cast.h"

#include <string.h>


#if ae2f_MAC_BUILD

ae2fVK_AnnSlpMk_t	mk;

size_t			mapinp;

ae2f_float_t FakeAct(ae2f_float_t v) { return 0; }

ae2f_float_t FakeLoss (
		const ae2f_float_t* out, 
		const ae2f_float_t* goal,
		size_t index,
		size_t count
		) { return 0; }

ae2f_float_t*	map;

int main() {
	Test_VkInit();
	(void)sizeof(ae2fVK_AnnSlpMk_t);
	(void)sizeof(ae2fVK_AnnSlp);

	__ae2fVK_AnnSlpMk_imp(
			mk
			, 0, 0, 0
			, 3, 3, 0, 0
			, FakeAct, FakeAct, FakeLoss
			, 0, 0
			, vkdev	
			, vkphydevmemprops
			, NULL

			, 
			"#define LOSS_DERIV(y, y_desired, i, c) 0\n"
			  "#define ACT(x) ((x) - 3)\n"
			, "/** This is also a comment */"
			);

	assert(mk.m_union.m_alter.m_ptr && "__ae2fVK_AnnSlpMk_imp has failed");
	assert(mk.m_reterr == ae2f_errGlob_OK);

	__ae2fVK_AnnSlpMap_imp(mk.m_reterr, *(mk).m_union.m_alter.m_ptr, &map);
	map[0] = 1;
	map[1] = 2;
	map[2] = 3;
	__ae2fVK_AnnSlpUnMap_imp(*(mk).m_union.m_alter.m_ptr);

	__ae2fVK_AnnSlpClean_imp(*mk.m_union.m_alter.m_ptr);
	assert(mk.m_union.m_alter.m_ptr->m_vkres == VK_SUCCESS);

	free(mk.m_union.m_alter.m_ptr);
	
	Test_VkEnd();
	mk.m_union.m_alter.m_ptr = 0;

	return 0;
}

#else

int main() {
	return 0;
}

#endif
