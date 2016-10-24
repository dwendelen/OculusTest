#pragma once

#include <vector>

#include "VulkanFwd.h"

using namespace std;

namespace vulkan
{
    class VulkanCommands
    {
    private:
        VulkanContext& context;
        VulkanDisplay& display;
        VulkanRenderPass& renderPass;
        VulkanDescriptors& descriptors;
        VulkanMemoryManager& memoryManager;

        vector<VkCommandBuffer> commandBuffers;
    public:
        VulkanCommands(
            VulkanContext& context,
            VulkanDisplay& display,
            VulkanRenderPass& renderPass,
            VulkanDescriptors& descriptors,
            VulkanMemoryManager& memoryManager);
        void init(int nbOfIndices);
		VkCommandBuffer getCommandBuffer(int i) { return commandBuffers[i]; };
        ~VulkanCommands();
    };
}
