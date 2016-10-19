#include "VulkanFwd.h"

namespace vulkan {
    class VulkanContext;
    class VulkanDescriptors
    {
        private:
            VulkanContext& context;
            VulkanRenderPass& renderPass;
            VkDescriptorPool pool;
            VkDescriptorSet uniformSet;
        public:
            VulkanDescriptors(VulkanContext& context, VulkanRenderPass& renderPass);
            void init();
            VkDescriptorSet getUniformSet() { return uniformSet; }
            ~VulkanDescriptors();
    };
}
