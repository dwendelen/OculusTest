#include "VulkanDescriptors.h"

#include "VulkanContext.h"
#include "VulkanRenderPass.h"

#include <stdexcept>

namespace vulkan {
    VulkanDescriptors::VulkanDescriptors(VulkanContext& context, VulkanRenderPass& renderPass):
        context(context),
        renderPass(renderPass),
        pool(VK_NULL_HANDLE),
        uniformSet(VK_NULL_HANDLE)
    {
        //ctor
    }

    void VulkanDescriptors::init()
    {
        VkResult r;

        VkDescriptorPoolSize sizeUniform = {};
        sizeUniform.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        sizeUniform.descriptorCount = 2;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.pNext = nullptr;
        poolInfo.flags = 0;
        poolInfo.maxSets = 1;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &sizeUniform;

        r = vkCreateDescriptorPool(context.getDevice(), &poolInfo, nullptr, &pool);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create descriptor pool");
        }

        VkDescriptorSetLayout layout = renderPass.getSetLayout();

        VkDescriptorSetAllocateInfo setInfo = {};
        setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        setInfo.pNext = nullptr;
        setInfo.descriptorPool = pool;
        setInfo.descriptorSetCount = 1;
        setInfo.pSetLayouts = &layout;

        r = vkAllocateDescriptorSets(context.getDevice(), &setInfo, &uniformSet);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not allocate descriptor set");
        }
    }

    VulkanDescriptors::~VulkanDescriptors()
    {
        /*if(uniformSet) {
            vkFreeDescriptorSets(context.getDevice(), pool, 1, &uniformSet);
            uniformSet = VK_NULL_HANDLE;
        }*/
        if(pool) {
            vkDestroyDescriptorPool(context.getDevice(), pool, nullptr);
            pool = VK_NULL_HANDLE;
        }
    }
}
