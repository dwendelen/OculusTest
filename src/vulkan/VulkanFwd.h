#pragma once

#define VK_USE_PLATFORM_XCB_KHR 1
#include "vulkan/vulkan.h"

namespace vulkan
{
    class VulkanContext;
    class VulkanCommands;
    class VulkanDebug;
    class VulkanDescriptors;
    class VulkanRenderPass;
    class VulkanDisplay;
    class VulkanMemoryManager;
}
