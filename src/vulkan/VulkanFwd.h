#pragma once
#include "SDL_platform.h"

#if defined(__WINDOWS__)
	#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#if defined(__LINUX__)
	#define VK_USE_PLATFORM_XCB_KHR 1
#endif
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
