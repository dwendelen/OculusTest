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
            VkQueue queue;
            uint32_t queueFamilyIndex;
            uint32_t memoryTypeIndex;
            VkCommandPool commandPool;

            void createInstance();
            void createDevice();
            VkPhysicalDevice getPhysicalDevice();
            uint32_t getQueueFamilyIndex(VkPhysicalDevice physicalDevice);
            uint32_t calculateMemoryTypeIndex(VkPhysicalDevice physicalDevice);
        public:
            VulkanContext();
            void init();
            VkInstance getInstance() { return instance; }
            VkDevice getDevice() { return device; }
            VkCommandPool getCommandPool() { return commandPool; }
            uint32_t getMemoryTypeIndex() { return memoryTypeIndex; }
            ~VulkanContext();
    };
}

