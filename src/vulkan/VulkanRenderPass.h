#pragma once

#include "VulkanFwd.h"

namespace vulkan
{
    class VulkanRenderPass
    {
        private:
            VulkanContext& vulkanContext;
            VkRenderPass renderPass;
            VkShaderModule vertexShader;
            VkShaderModule fragmentShader;
            VkDescriptorSetLayout setLayout;
            VkPipelineLayout layout;
            VkPipeline pipeline;

            int width;
            int height;
        public:
            VulkanRenderPass(VulkanContext& vulkanContext, int width, int height);
            void init();
            VkRenderPass getRenderPass() { return renderPass; }
            VkPipeline getPipeline() { return pipeline; }
            VkPipelineLayout getLayout() { return layout; }
            VkDescriptorSetLayout getSetLayout() { return setLayout; }
            ~VulkanRenderPass();
    };
}
