#pragma once

#define VK_USE_PLATFORM_XCB_KHR 1
#include "vulkan/vulkan.h"
#include <vector>

using namespace std;

namespace vulkan
{
    class VulkanContext;
    class VulkanDisplay;
    class VulkanRenderPass;

    class VulkanCommands
    {
    private:
        VulkanContext& context;
        VulkanDisplay& display;
        VulkanRenderPass& renderPass;
        vector<VkCommandBuffer> commandBuffers;
    public:
        VulkanCommands(VulkanContext& context, VulkanDisplay& display, VulkanRenderPass& renderPass);
        void init();
        ~VulkanCommands();
    };
}
