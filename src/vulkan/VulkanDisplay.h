#pragma once
#include "Display.h"
#include "SDL2/SDL.h"
#define VK_USE_PLATFORM_XCB_KHR 1
#include "vulkan/vulkan.h"

using namespace video;

namespace vulkan
{
    class VulkanContext;

    class VulkanDisplay: public video::Displayy //todo better error-handling + cross platform
    {
    private:
        VulkanContext& vulkanContext;
        SDL_Window* window;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        int height;
        int width;
    public:
        VulkanDisplay(VulkanContext& vulkanContext, int width, int height);
        virtual void swap();
        void init();
        virtual void prepareForNewFrame() {}
        virtual int getWidth() {return 0;};
        virtual int getHeight() { return 0;};
        virtual ~VulkanDisplay();
    };
}
