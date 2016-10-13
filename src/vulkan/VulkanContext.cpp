#include "VulkanContext.h"

#include "VulkanDebug.h"
#include <vector>
#include <exception>

namespace vulkan
{
    template<typename E, typename R>
    vector<R> getVector(
        VkResult(*func)(E, uint32_t*, R*),
        E entity, string errorMessage)
    {
        uint32_t count;
        if(func(entity, &count, nullptr) != VK_SUCCESS) {
            throw new runtime_error(errorMessage);
        }
        vector<R> result(count);
        if(func(entity, &count, result.data()) != VK_SUCCESS) {
            throw new runtime_error(errorMessage);
        }
        return result;
    }

    template<typename E, typename R>
    vector<R> getVector(void(*func)(E, uint32_t*, R*), E entity)
    {
        uint32_t count;
        func(entity, &count, nullptr);
        vector<R> result(count);
        func(entity, &count, result.data());
        return result;
    }

    VulkanContext::VulkanContext():
        instance(VK_NULL_HANDLE),
        device(VK_NULL_HANDLE),
        queue(VK_NULL_HANDLE) {}

    void VulkanContext::init()
    {
        createInstance();
        debug = unique_ptr<VulkanDebug>(new VulkanDebug(instance));
        debug->init();

        createDevice();
        vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
    }

    void VulkanContext::createInstance()
    {
        vector<const char*> layers = {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_image",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_LUNARG_swapchain",
            "VK_LAYER_GOOGLE_unique_objects"
        };
        vector<const char*> extensions = {
            VK_KHR_XCB_SURFACE_EXTENSION_NAME,
            "VK_KHR_surface",
            "VK_EXT_debug_report"
        };

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 8);
        appInfo.pEngineName = "Engine";
        appInfo.engineVersion = 0;
        appInfo.pApplicationName = "Application";
        appInfo.applicationVersion = 0;
        appInfo.pNext = nullptr;

        VkInstanceCreateInfo instInfo = {};
        instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instInfo.enabledExtensionCount = extensions.size();
        instInfo.ppEnabledExtensionNames = extensions.data();
        instInfo.enabledLayerCount = layers.size();
        instInfo.ppEnabledLayerNames = layers.data();
        instInfo.pNext = nullptr;
        instInfo.pApplicationInfo = &appInfo;
        instInfo.flags = 0;

        if(vkCreateInstance(&instInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance!");
        }
    }
    void VulkanContext::createDevice()
    {
        vector<const char*> layers = {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_image",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_LUNARG_swapchain",
            "VK_LAYER_GOOGLE_unique_objects"
        };
        vector<const char*> extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkPhysicalDevice physicalDevice = getPhysicalDevice();
        queueFamilyIndex = getQueueFamilyIndex(physicalDevice);

        float prio[] = {1.0f};
        VkDeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.flags = 0;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.pQueuePriorities = prio;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.enabledExtensionCount = extensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
        deviceCreateInfo.enabledLayerCount = layers.size();
        deviceCreateInfo.ppEnabledLayerNames = layers.data();
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.pEnabledFeatures = nullptr;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
        deviceCreateInfo.queueCreateInfoCount = 1;

        if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
            throw new runtime_error("Could not create a device");
        }
    }
    VkPhysicalDevice VulkanContext::getPhysicalDevice()
    {
        vector<VkPhysicalDevice> devices = getVector(vkEnumeratePhysicalDevices, instance,
            "Could not enumerate over the physical devices");

        for(VkPhysicalDevice device: devices)
        {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(device, &props);
            if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                return device;
            }
        }

        throw new std::runtime_error("Could not find a discrete GPU");
    }

    uint32_t VulkanContext::getQueueFamilyIndex(VkPhysicalDevice device)
    {
        uint32_t index = 0;
        vector<VkQueueFamilyProperties> families = getVector(vkGetPhysicalDeviceQueueFamilyProperties, device);
        for(VkQueueFamilyProperties family: families) {
            if(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                return index;
            }
            index++;
        }

        throw new runtime_error("No graphics queue found");
    }
    VulkanContext::~VulkanContext()
    {
        debug.reset(); //Must be destroyed before the instance
        if(device) {
            vkDestroyDevice(device, nullptr);
        }
        if(instance) {
            vkDestroyInstance(instance, nullptr);
        }
    }
}
