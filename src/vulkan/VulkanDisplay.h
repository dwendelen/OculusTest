#pragma once
#include "Display.h"
#include "SDL.h"

#include <vector>
#include <VulkanUtils.h>

#include "VulkanFwd.h"

using namespace std;
using namespace video;

namespace vulkan
{
    class VulkanDisplay: public video::Displayy //todo better error-handling + cross platform
    {
    private:
        VulkanContext& vulkanContext;
        VulkanRenderPass& renderPass;
		VulkanMemoryManager& memoryManager;

        SDL_Window* window;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        vector<VkImageView> views;
        vector<VkFramebuffer> framebuffers;

        int height;
        int width;
		uint32_t swapIndex;

		VkSemaphore renderingDone;
		VkSemaphore imageReady;
    public:
        VulkanDisplay(VulkanContext& vulkanContext, VulkanRenderPass& renderPass, VulkanMemoryManager& memoryManager, int width, int height);
        virtual void swap();
        void init();
        vector<VkFramebuffer> getFramebuffers() { return framebuffers; }
		virtual void prepareForNewFrame();
        virtual int getWidth() { return width; };
        virtual int getHeight() { return height; };
		void setSemaphores(VkSemaphore beforeRendering, VkSemaphore renderingDone)
		{
			imageReady = beforeRendering;
			this->renderingDone = renderingDone;
		}
		int getSwapChainIndex() { return swapIndex; };
		VkSemaphore getRenderingDone() { return renderingDone; }
		VkSemaphore getImageReady() { return imageReady; }
        virtual ~VulkanDisplay();
    };
}
