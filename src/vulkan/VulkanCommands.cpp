#include "VulkanCommands.h"

#include "VulkanContext.h"
#include "VulkanDisplay.h"
#include "VulkanRenderPass.h"

namespace vulkan
{
    VulkanCommands::VulkanCommands(VulkanContext& context, VulkanDisplay& display, VulkanRenderPass& renderPass):
        context(context),
        display(display),
        renderPass(renderPass)
    {

    }
    void VulkanCommands::init()
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

        for(int i = 0; i < display.getFramebuffers().size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr; // Optional

            r = vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
            if(r != VK_SUCCESS) {
                throw new runtime_error("Could not begin recording command");
            }

            VkClearValue clearValue[0];
            clearValue[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValue[1].depthStencil = {1.0f, 0};

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.pNext = nullptr;
            renderPassInfo.renderPass = renderPass.getRenderPass();
            renderPassInfo.framebuffer = display.getFramebuffers()[i];
            renderPassInfo.renderArea.extent.height = display.getHeight();
            renderPassInfo.renderArea.extent.width = display.getWidth();
            renderPassInfo.renderArea.offset.x = 0;
            renderPassInfo.renderArea.offset.y = 0;
            renderPassInfo.clearValueCount = 2;
            renderPassInfo.pClearValues = clearValue;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderPass.getPipeline());

            //vkCmdBindDescriptorSets()
            //vkCmdBindVertexBuffers()
            //vkCmdBindIndexBuffer()
            //vkCmdDrawIndexed(commandBuffers[i], )
            vkCmdEndRenderPass(commandBuffers[i]);
            r = vkEndCommandBuffer(commandBuffers[i]);
            if(r != VK_SUCCESS) {
                throw new runtime_error("Could not finish recording command buffers");
            }
        }
    }
    VulkanCommands::~VulkanCommands(){}
}
