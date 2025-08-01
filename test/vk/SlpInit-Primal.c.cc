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
	sizeof(ae2fVK_AnnSlpMk_t);
	sizeof(ae2fVK_AnnSlp);

	__ae2fVK_AnnSlpMk_imp(
			mk
			, 0, 0, 0
			, 3, 3, 0, 0
			, FakeAct, FakeAct, FakeLoss
			, 0, 0
			, vkdev	
			, vkphydevmemprops
			, NULL
			, ae2f_reinterpret_cast(const VkBufferCreateInfo*, 0)
			, ae2f_reinterpret_cast(const VkMemoryAllocateInfo*, 0)

			, "#define LOSS_DERIV(y, y_desired, i, c) 0\n"
			, "/** This is also a comment */"
			);

	assert(mk.m_union.m_alter.m_ptr && "__ae2fVK_AnnSlpMk_imp has failed");
	assert(mk.m_reterr == ae2f_errGlob_OK);

	__ae2fVK_AnnSlpMap(*(mk).m_union.m_alter.m_ptr, &map);
	__ae2fVK_AnnSlpUnMap(*(mk).m_union.m_alter.m_ptr);

	__ae2fVK_AnnSlpClean(*mk.m_union.m_alter.m_ptr);
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
