#pragma once

#include <memory>

#include "VulkanFwd.h"

using namespace std;

namespace vulkan
{
    class VulkanContext //TODO: enumerate extensions, layers for device and instance
    {
        private:
            VkInstance instance;
            unique_ptr<VulkanDebug> debug;
            VkDevice device;
            VkPhysicalDevice physicalDevice;
            VkQueue queue;
            uint32_t queueFamilyIndex;
            VkCommandPool commandPool;

            void createInstance();
            void createDevice();
            VkPhysicalDevice getPhysicalDeviceFromVulkan();
            uint32_t getQueueFamilyIndex(VkPhysicalDevice physicalDevice);
            uint32_t calculateMemoryTypeIndex(VkPhysicalDevice physicalDevice);
        public:
            VulkanContext();
            void init();
            VkInstance getInstance() { return instance; }
            VkDevice getDevice() { return device; }
            VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
            VkCommandPool getCommandPool() { return commandPool; }
			VkQueue getQueue() { return queue; }
            ~VulkanContext();
    };
}

