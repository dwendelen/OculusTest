#include "VulkanMemoryManager.h"

#include "VulkanContext.h"
#include "VulkanDescriptors.h"

#include <vector>
#include "Model.h"

namespace vulkan
{
    VulkanMemoryManager::VulkanMemoryManager(VulkanContext& context, VulkanDescriptors& descriptors):
        context(context),
        descriptors(descriptors),
        vertexBuffer(VK_NULL_HANDLE),
        vertexMemory(VK_NULL_HANDLE),
        indicesBuffer(VK_NULL_HANDLE),
        indicesMemory(VK_NULL_HANDLE),
        uniformBuffer(VK_NULL_HANDLE),
        uniformMemory(VK_NULL_HANDLE)
    {
        //ctor
    }

    struct matr {
        Matrix4f mvp;
        Matrix4f rot;
    };


    void VulkanMemoryManager::init()
    {
        allocateMemory(vertexBuffer, vertexMemory,
            4 * 1024 * 1024, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, "vertex");
        allocateMemory(indicesBuffer, indicesMemory,
            1 * 1024 * 1024, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, "indices");
        allocateMemory(uniformBuffer, uniformMemory,
            1 * 1024 * 1024, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, "uniform");

        VkDescriptorBufferInfo bufferInfo[] = {{}, {}};
        bufferInfo[0].buffer = uniformBuffer;
        bufferInfo[0].offset = 0;
        bufferInfo[0].range = sizeof(matr);

        bufferInfo[1].buffer = uniformBuffer;
        bufferInfo[1].offset = sizeof(matr);
        bufferInfo[1].range = sizeof(Vector4f);

        VkWriteDescriptorSet write = {};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstSet = descriptors.getUniformSet();
        write.dstBinding = 0;
        write.dstArrayElement = 0;
        write.descriptorCount = 2;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pImageInfo = nullptr;
        write.pBufferInfo = bufferInfo;
        write.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(context.getDevice(), 2, &write, 0, nullptr);
    }

    void VulkanMemoryManager::allocateMemory(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, string objectName)
    {
        VkResult r;

        VkBufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.pNext = nullptr;
        vertexBufferInfo.flags = 0;
        vertexBufferInfo.size = size;
        vertexBufferInfo.usage = usage;
        vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vertexBufferInfo.queueFamilyIndexCount = 0;
        vertexBufferInfo.pQueueFamilyIndices = nullptr;

        r = vkCreateBuffer(context.getDevice(), &vertexBufferInfo, nullptr, &buffer);
        if(r != VK_SUCCESS) {
           throw new runtime_error("Could not create " + objectName + " buffer");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(context.getDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo memoryInfo = {};
        memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryInfo.pNext = nullptr;
        memoryInfo.allocationSize = memRequirements.size;
        memoryInfo.memoryTypeIndex = context.getMemoryTypeIndex();

        r = vkAllocateMemory(context.getDevice(), &memoryInfo, nullptr, &memory);
        if(r != VK_SUCCESS) {
           throw new runtime_error("Could not create " + objectName + " memory");
        }

        r = vkBindBufferMemory(context.getDevice(), buffer, memory, 0);
        if(r != VK_SUCCESS) {
           throw new runtime_error("Could not bind " + objectName + " memory and " + objectName + " buffer");
        }
    }

    template<typename T>
    void upload(VkDevice device, vector<T> src, VkDeviceMemory memory, string objectName)
    {
        void* dest;
        int size = src.size() * sizeof(T);
        VkResult r = vkMapMemory(device, memory, 0, size, 0, &dest);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not upload " + objectName);
        }
        memcpy(dest, src.data(), size);
        vkUnmapMemory(device, memory);
    }

    void VulkanMemoryManager::load(const Model& model)
    {
        upload(context.getDevice(), model.getIndices(), indicesMemory, "indices");
        upload(context.getDevice(), model.getVertices(), vertexMemory, "vertices");
    }

    void VulkanMemoryManager::setUniform(Matrix4f mvp, Matrix4f rot, Vector4f color)
    {
        struct matrCol {
            struct matr m;
            Vector4f col;
        };

        struct matrCol m = {mvp, rot, color};

        void* dest;
        int size = sizeof(matrCol);
        VkResult r = vkMapMemory(context.getDevice(), uniformMemory, 0, size, 0, &dest);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not upload uniform");
        }
        memcpy(dest, &m, size);
        vkUnmapMemory(context.getDevice(), uniformMemory);
    }

    VulkanMemoryManager::~VulkanMemoryManager()
    {
        if(vertexBuffer) {
            vkDestroyBuffer(context.getDevice(), vertexBuffer, nullptr);
            vertexBuffer = VK_NULL_HANDLE;
        }
        if(vertexMemory) {
            vkFreeMemory(context.getDevice(), vertexMemory, nullptr);
            vertexMemory = VK_NULL_HANDLE;
        }

        if(uniformBuffer) {
            vkDestroyBuffer(context.getDevice(), uniformBuffer, nullptr);
            uniformBuffer = VK_NULL_HANDLE;
        }
        if(uniformMemory) {
            vkFreeMemory(context.getDevice(), uniformMemory, nullptr);
            uniformMemory = VK_NULL_HANDLE;
        }

        if(indicesBuffer) {
            vkDestroyBuffer(context.getDevice(), indicesBuffer, nullptr);
            indicesBuffer = VK_NULL_HANDLE;
        }
        if(indicesMemory) {
            vkFreeMemory(context.getDevice(), indicesMemory, nullptr);
            indicesMemory = VK_NULL_HANDLE;
        }
    }
}
