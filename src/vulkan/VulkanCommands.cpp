#include "VulkanCommands.h"

#include "VulkanContext.h"
#include "VulkanDisplay.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptors.h"
#include "VulkanMemoryManager.h"

namespace vulkan
{
    VulkanCommands::VulkanCommands(
        VulkanContext& context,
        VulkanDisplay& display,
        VulkanRenderPass& renderPass,
        VulkanDescriptors& descriptors,
        VulkanMemoryManager& memoryManager)
        :
        context(context),
        display(display),
        renderPass(renderPass),
        descriptors(descriptors),
        memoryManager(memoryManager)
    {

    }
    void VulkanCommands::init(int nbOfIndices)
    {
        VkCommandBufferAllocateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        bufferInfo.pNext = nullptr;
        bufferInfo.commandPool = context.getCommandPool();
        bufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        bufferInfo.commandBufferCount = display.getFramebuffers().size();

        commandBuffers.resize(display.getFramebuffers().size(), VK_NULL_HANDLE);
        VkResult r = vkAllocateCommandBuffers(context.getDevice(), &bufferInfo, commandBuffers.data());
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create command buffers");
        }

        vector<VkFramebuffer> framebuffers = display.getFramebuffers();
        for(int i = 0; i < display.getFramebuffers().size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            r = vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
            if(r != VK_SUCCESS) {
                throw new runtime_error("Could not begin recording command");
            }

            VkClearValue clearValue[1];
            clearValue[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            //clearValue[1].depthStencil = {1.0f, 0};

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.pNext = nullptr;
            renderPassInfo.renderPass = renderPass.getRenderPass();
            renderPassInfo.framebuffer = framebuffers[i];
            renderPassInfo.renderArea.extent.height = display.getHeight();
            renderPassInfo.renderArea.extent.width = display.getWidth();
            renderPassInfo.renderArea.offset.x = 0;
            renderPassInfo.renderArea.offset.y = 0;
            renderPassInfo.clearValueCount = 1;
            //renderPassInfo.clearValueCount = 2;
            renderPassInfo.pClearValues = clearValue;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderPass.getPipeline());

            VkBuffer vertexBuffer = memoryManager.getVertexBuffer();
            VkDeviceSize zeroOffset = 0;
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &vertexBuffer, &zeroOffset);
            vkCmdBindIndexBuffer(commandBuffers[i], memoryManager.getIndicesBuffer(), 0, VK_INDEX_TYPE_UINT32);

/*
            VkDescriptorSet descSet = descriptors.getUniformSet();

            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                renderPass.getLayout(), 0, 1, &descSet, 0, nullptr);
*/
            vkCmdDrawIndexed(commandBuffers[i], nbOfIndices, 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            r = vkEndCommandBuffer(commandBuffers[i]);
            if(r != VK_SUCCESS) {
                throw new runtime_error("Could not finish recording command buffers");
            }
        }
    }
    VulkanCommands::~VulkanCommands(){}
}
