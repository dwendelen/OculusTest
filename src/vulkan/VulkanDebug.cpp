#include "VulkanDebug.h"

#include <iostream>
#include <exception>

using namespace std;
namespace vulkan
{
    VulkanDebug::VulkanDebug(VkInstance instance):
        instance(instance)
    {
        vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>
            (vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
        vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>
            (vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT"));
        vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
            (vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    }
    VkBool32 VulkanDebug_ReportCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* pLayerPrefix,
		const char* pMsg,
		void* pUserData)
	{
        cerr << pMsg << endl;
        return VK_FALSE;
	}

    void VulkanDebug::init()
    {
        VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
        callbackCreateInfo.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        callbackCreateInfo.pNext       = nullptr;
        callbackCreateInfo.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                         VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                         VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = &VulkanDebug_ReportCallback;
        callbackCreateInfo.pUserData   = nullptr;

        VkResult result = vkCreateDebugReportCallbackEXT(instance, &callbackCreateInfo, nullptr, &callback);
        if(result != VK_SUCCESS) {
            throw new runtime_error("Could not create debug report callback");
        }
    }
    VulkanDebug::~VulkanDebug() {
        if(callback) {
            vkDestroyDebugReportCallbackEXT(instance, callback, nullptr);
            callback = VK_NULL_HANDLE;
        }
    }
}
