#pragma once
#include "Display.h"
#include "SDL2/SDL.h"

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

        SDL_Window* window;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        vector<VkImageView> views;
        vector<VkFramebuffer> framebuffers;
        VkImage depthImage;
        VkDeviceMemory depthMemory;
        VkImageView depthView;

        int height;
        int width;
    public:
        VulkanDisplay(VulkanContext& vulkanContext, VulkanRenderPass& renderPass, int width, int height);
        virtual void swap();
        void init();
        vector<VkFramebuffer> getFramebuffers() { return framebuffers; }
        VkImageView getDepthView() { return depthView; }
        virtual void prepareForNewFrame() {}
        virtual int getWidth() { return width; };
        virtual int getHeight() { return height; };
        virtual ~VulkanDisplay();
    };
}
