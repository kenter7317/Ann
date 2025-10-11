#include <vulkan/vulkan_core.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

VkResult vkres = VK_SUCCESS;

static VkInstanceCreateInfo	vulkancreat;
static VkInstance		vulkan;

static VkPhysicalDevice			vkphydev;
static uint32_t				vkphydevcount = 0;
static VkPhysicalDeviceMemoryProperties	vkphydevmemprops;

static VkDevice			vkdev;
static VkDeviceCreateInfo	vkdevcreat;
static VkQueue			vkqueue;

static uint32_t find_queue_family(VkPhysicalDevice phydev) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
                        phydev
                        , &queueFamilyCount
                        , NULL
                        );

        union upVkQueueFamilyProperties {
                void*   v;
                VkQueueFamilyProperties* p;
        } queueFamilies;  
                        
        queueFamilies.v = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));

        vkGetPhysicalDeviceQueueFamilyProperties(
                        phydev
                        , &queueFamilyCount
                        , queueFamilies.p
                        );

        uint32_t queueFamilyIndex = UINT32_MAX;
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
                if (queueFamilies.p[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                        queueFamilyIndex = i;
                        break;
                }
        }
        free(queueFamilies.v);
        return queueFamilyIndex;
}

static void Test_VkInit() {
        vkdev = 0;
        vulkan = 0;
        vkphydevcount = 0;
        vkphydev = 0;
        memset(&vkphydevmemprops, 0, sizeof(vkphydevmemprops));
        memset(&vkdevcreat, 0, sizeof(vkdevcreat));
        memset(&vulkancreat, 0, sizeof(vulkancreat));


        vulkancreat.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vulkancreat.enabledExtensionCount = 0;
        vulkancreat.enabledLayerCount = 0;
        vulkancreat.ppEnabledExtensionNames = 0;
        vulkancreat.ppEnabledLayerNames = 0;
        vulkancreat.flags = 0;
        vulkancreat.pApplicationInfo = NULL;

        vkres = vkCreateInstance(&vulkancreat, 0, &vulkan);
        printf("vkCreateInstance result: %d\n", vkres);
        assert(vkres == VK_SUCCESS && "vkCreateInstance has failed");

        vkphydevcount = 0;

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

        assert(vkres == VK_SUCCESS || vkres == VK_INCOMPLETE && "vkEnumeratePhysicalDevices has failed.");
        assert(vkphydevcount != 0 && "vkphydevcount has changed, which is not expected.");
        assert(vkphydev && "vkphydev is no initialised");

        vkphydevcount = 1;

        vkdevcreat.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        // Find a queue family
        uint32_t queueFamilyIndex = find_queue_family(vkphydev);
        assert(queueFamilyIndex != UINT32_MAX && "No suitable queue family found");

        // Initialize VkDeviceQueueCreateInfo
        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo;
        memset(&queueCreateInfo, 0, sizeof(queueCreateInfo));

        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        // Initialize VkDeviceCreateInfo
        vkdevcreat.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        vkdevcreat.queueCreateInfoCount = 1;
        vkdevcreat.pQueueCreateInfos = &queueCreateInfo;
        vkdevcreat.enabledExtensionCount = 0;
        vkdevcreat.ppEnabledExtensionNames = NULL;
        vkdevcreat.enabledLayerCount = 0;
        vkdevcreat.ppEnabledLayerNames = NULL;
        vkdevcreat.flags = 0;

        vkres = vkCreateDevice(
                        vkphydev
                        , &vkdevcreat
                        , 0
                        , &vkdev
                        );

	vkGetDeviceQueue(
			vkdev
			, queueFamilyIndex
			, 0
			, &vkqueue
			);

        assert(vkres == VK_SUCCESS && "vkCreateDevice has failed.");
        assert(vkdev && "vkdev is not initialised");

        vkGetPhysicalDeviceMemoryProperties(
                        vkphydev
                        , &vkphydevmemprops
                        );

	puts("Test_VkInit has done.");
}

static void Test_VkEnd() {
        if(vkdev)	vkDestroyDevice(vkdev, 0);
        if(vulkan)	vkDestroyInstance(vulkan, 0);
}
