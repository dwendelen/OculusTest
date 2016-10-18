#pragma once
#include <vulkan/vulkan.h>

namespace vulkan
{
    class VulkanContext;
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

                void setupShaders(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupVertexInput(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupInputAssembly(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupViewport(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupRastization(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupMultisample(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupDepth(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupColorBlend(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupLayout(VkGraphicsPipelineCreateInfo& pipelineInfo);
                void setupRenderPass(VkGraphicsPipelineCreateInfo& pipelineInfo);
        public:
            VulkanRenderPass(VulkanContext& vulkanContext, int width, int height);
            void init();
            VkRenderPass getRenderPass() { return renderPass; }
            VkPipeline getPipeline() { return pipeline; }
            VkPipelineLayout getLayout() { return layout; }
            ~VulkanRenderPass();
    };
}
