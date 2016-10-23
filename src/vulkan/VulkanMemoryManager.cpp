#include "VulkanMemoryManager.h"

#include "VulkanContext.h"
#include "VulkanDescriptors.h"
#include "VulkanDisplay.h"

#include <vector>
#include "Model.h"
#include <memory>

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
        uniformMemory(VK_NULL_HANDLE),
		depthImage(VK_NULL_HANDLE),
		depthMemory(VK_NULL_HANDLE),
		depthView(VK_NULL_HANDLE),
		colorUniformOffset(0)
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

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(context.getPhysicalDevice(), &props);
		int align = props.limits.minUniformBufferOffsetAlignment;
		colorUniformOffset = ((sizeof(matr) + align - 1) / align) * align;
			
        VkDescriptorBufferInfo bufferInfo[] = {{}, {}};
        bufferInfo[0].buffer = uniformBuffer;
        bufferInfo[0].offset = 0;
        bufferInfo[0].range = sizeof(matr);

        bufferInfo[1].buffer = uniformBuffer;
		bufferInfo[1].offset = colorUniformOffset;
        bufferInfo[1].range = sizeof(Vector4f);

        VkWriteDescriptorSet write[] = {{},{}};
        write[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write[0].pNext = nullptr;
        write[0].dstSet = descriptors.getUniformSet();
        write[0].dstBinding = 0;
        write[0].dstArrayElement = 0;
        write[0].descriptorCount = 1;
        write[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write[0].pImageInfo = nullptr;
        write[0].pBufferInfo = &bufferInfo[0];
        write[0].pTexelBufferView = nullptr;

        write[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write[1].pNext = nullptr;
        write[1].dstSet = descriptors.getUniformSet();
        write[1].dstBinding = 1;
        write[1].dstArrayElement = 0;
        write[1].descriptorCount = 1;
        write[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write[1].pImageInfo = nullptr;
        write[1].pBufferInfo = &bufferInfo[1];
        write[1].pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(context.getDevice(), 2, write, 0, nullptr);
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
        memoryInfo.memoryTypeIndex = calculateMemoryTypeIndex(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        r = vkAllocateMemory(context.getDevice(), &memoryInfo, nullptr, &memory);
        if(r != VK_SUCCESS) {
           throw new runtime_error("Could not create " + objectName + " memory");
        }

        r = vkBindBufferMemory(context.getDevice(), buffer, memory, 0);
        if(r != VK_SUCCESS) {
           throw new runtime_error("Could not bind " + objectName + " memory and " + objectName + " buffer");
        }
    }

    uint32_t VulkanMemoryManager::calculateMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags requiredFlags)
    {
        VkPhysicalDeviceMemoryProperties props;
        vkGetPhysicalDeviceMemoryProperties(context.getPhysicalDevice(), &props);
        uint32_t typeBit = 1;
        for(int i = 0; i < props.memoryTypeCount; i++)
        {
            if ((typeBits & typeBit)) {
                VkMemoryPropertyFlags flags = props.memoryTypes[i].propertyFlags;
                if((flags & requiredFlags) == requiredFlags) {
                    return i;
                }
            }

            typeBit = typeBit << 1;
        }

        throw new runtime_error("No appropriate device found");
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

        VkMappedMemoryRange range = {};
        range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range.pNext = nullptr;
        range.memory = memory;
        range.offset = 0;
        range.size = size;
        r = vkFlushMappedMemoryRanges(device, 1, &range);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not upload " + objectName);
        }

        vkUnmapMemory(device, memory);
    }

    void VulkanMemoryManager::load(const Model& model)
    {
        upload(context.getDevice(), model.getIndices(), indicesMemory, "indices");
        upload(context.getDevice(), model.getVertices(), vertexMemory, "vertices");
    }

    void VulkanMemoryManager::setUniform(Matrix4f mvp, Matrix4f rot, Vector4f color)
    {
        struct matr m = {mvp.Transposed(), rot.Transposed()};

        void* dest;
        int size = colorUniformOffset + sizeof(color);
        VkResult r = vkMapMemory(context.getDevice(), uniformMemory, 0, size, 0, &dest);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not upload uniform");
        }
        memcpy(dest, &m, sizeof(m));
		memcpy(static_cast<byte*>(dest) + colorUniformOffset, &color, sizeof(color));
        VkMappedMemoryRange range = {};
        range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range.pNext = nullptr;
        range.memory = uniformMemory;
        range.offset = 0;
        range.size = size;
        r = vkFlushMappedMemoryRanges(context.getDevice(), 1, &range);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not upload uniform");
        }

        vkUnmapMemory(context.getDevice(), uniformMemory);
    }

	void VulkanMemoryManager::createDepthBuffer(int width, int height) {
		VkResult r;
		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = VK_FORMAT_D32_SFLOAT;
		createInfo.extent.height = height;
		createInfo.extent.width = width;
		createInfo.extent.depth = 1;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		r = vkCreateImage(context.getDevice(), &createInfo, nullptr, &depthImage);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not create depth image");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(context.getDevice(), depthImage, &memRequirements);

		VkMemoryAllocateInfo memoryInfo = {};
		memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryInfo.pNext = nullptr;
		memoryInfo.allocationSize = memRequirements.size;
		memoryInfo.memoryTypeIndex = calculateMemoryTypeIndex(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		r = vkAllocateMemory(context.getDevice(), &memoryInfo, nullptr, &depthMemory);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not create depth memory");
		}

		r = vkBindImageMemory(context.getDevice(), depthImage, depthMemory, 0);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not bind depth memory and depthimage");
		}

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.pNext = nullptr;
		viewInfo.flags = 0;
		viewInfo.image = depthImage;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_D32_SFLOAT;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		r = vkCreateImageView(context.getDevice(), &viewInfo, nullptr, &depthView);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not create depth image view");
		}
	}

    VulkanMemoryManager::~VulkanMemoryManager()
    {
		if (depthView) {
			vkDestroyImageView(context.getDevice(), depthView, nullptr);
			depthView = VK_NULL_HANDLE;
		}
		if (depthMemory) {
			vkFreeMemory(context.getDevice(), depthMemory, nullptr);
			depthMemory = VK_NULL_HANDLE;
		}
		if (depthImage) {
			vkDestroyImage(context.getDevice(), depthImage, nullptr);
			depthImage = VK_NULL_HANDLE;
		}
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
