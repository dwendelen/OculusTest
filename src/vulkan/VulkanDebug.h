#pragma once
#define VK_USE_PLATFORM_XCB_KHR 1
#include "vulkan/vulkan.h"

namespace vulkan
{
    class VulkanDebug
    {
        private:
            PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
            PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
            PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
            VkDebugReportCallbackEXT callback = VK_NULL_HANDLE;
            VkInstance instance;
        public:
            VulkanDebug(VkInstance instance);
            void init();
            ~VulkanDebug();
    };
}
