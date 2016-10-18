#pragma once

#define VK_USE_PLATFORM_XCB_KHR 1
#include "vulkan/vulkan.h"
#include <memory>

using namespace std;
namespace vulkan
{
    class VulkanDebug;
    class VulkanContext //TODO: enumerate extensions, layers for device and instance
    {
        private:
            VkInstance instance;
            unique_ptr<VulkanDebug> debug;
            VkDevice device;
            VkQueue queue;
            uint32_t queueFamilyIndex;
            VkCommandPool commandPool;

            void createInstance();
            void createDevice();
            VkPhysicalDevice getPhysicalDevice();
            uint32_t getQueueFamilyIndex(VkPhysicalDevice physicalDevice);
        public:
            VulkanContext();
            void init();
            VkInstance getInstance() { return instance; }
            VkDevice getDevice() { return device; }
            VkCommandPool getCommandPool() { return commandPool; }
            ~VulkanContext();
    };
}

