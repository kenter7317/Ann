#include <vulkan/vulkan_core.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

VkResult vkres = 0;

static VkInstanceCreateInfo	vulkancreat = {0, };
static VkInstance		vulkan;

static VkPhysicalDevice			vkphydev;
static uint32_t				vkphydevcount = 0;
static VkPhysicalDeviceMemoryProperties	vkphydevmemprops;

static VkDevice			vkdev = 0;
static VkDeviceCreateInfo	vkdevcreat;

static void Test_VkInit() {
	vulkancreat.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vulkancreat.enabledExtensionCount = 0;
	vulkancreat.enabledLayerCount = 0;
	vulkancreat.ppEnabledExtensionNames = 0;
	vulkancreat.ppEnabledLayerNames = 0;
	vulkancreat.flags = 0;

	vkres = vkCreateInstance(&vulkancreat, 0, &vulkan);
	printf("vkCreateInstance result: %d\n", vkres);
	assert(vkres == VK_SUCCESS && "vkCreateInstance has failed");

	vkres = vkEnumeratePhysicalDevices(
			vulkan
			, &vkphydevcount
			, 0
			);

	assert(vkres == VK_SUCCESS);
	assert(vkphydevcount && "vkphydevcount must be greater than 0.");
	printf("Number of Physical Device available: %u\n", vkphydevcount);

	vkphydevcount = 1;

	vkres = vkEnumeratePhysicalDevices(
			vulkan
			, &vkphydevcount
			, &vkphydev
			);

	assert(vkres == VK_SUCCESS && "vkEnumeratePhysicalDevices has failed.");
	assert(vkphydevcount == 1 && "vkphydevcount has changed, which is not expected.");
	assert(vkphydev && "vkphydev is no initialised");

	vkdevcreat.flags = VK_API_VERSION_1_3;

	vkres = vkCreateDevice(
			vkphydev
			, &vkdevcreat
			, 0
			, &vkdev
			);
	
	assert(vkres == VK_SUCCESS && "vkCreateDevice has failed.");
	assert(vkdev && "vkdev is not initialised");

	vkGetPhysicalDeviceMemoryProperties(
			vkphydev
			, &vkphydevmemprops
			);
}

static void Test_VkEnd() {
	if(vkdev)	vkDestroyDevice(vkdev, 0);
	if(vulkan)	vkDestroyInstance(vulkan, 0);
}
