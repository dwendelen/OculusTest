#include "VulkanDisplay.h"

#include "VulkanContext.h"
#include "SDL2/SDL_syswm.h"
#include <X11/Xlib-xcb.h>
#include <exception>
#include <string>

#define VK_USE_PLATFORM_XCB_KHR 1
#include "vulkan/vulkan.h"

using namespace std;

namespace vulkan
{
    VulkanDisplay::VulkanDisplay(VulkanContext& vulkanContext, int width, int height):
        vulkanContext(vulkanContext),
        surface(VK_NULL_HANDLE),
        swapChain(VK_NULL_HANDLE),
        window(nullptr),
        height(height),
        width(width)
    {}

    void VulkanDisplay::init() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
            throw new runtime_error("Could not init SDL: " + string(SDL_GetError()));
        }

        window = SDL_CreateWindow("Vulkan window", 100, 100, 800, 600, 0);
        if (window == NULL) {
            throw new runtime_error("Could not create window:" + string(SDL_GetError()));
        }

        SDL_SysWMinfo windowInfo;
        SDL_VERSION(&windowInfo.version);
        SDL_GetWindowWMInfo(window, &windowInfo);
        if(windowInfo.subsystem != SDL_SYSWM_X11) {
            throw new runtime_error("Not Xlib");
        }

        PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR  = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>
                (vkGetInstanceProcAddr(vulkanContext.getInstance(), "vkCreateXcbSurfaceKHR"));

        VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.pNext = nullptr;
        surfaceCreateInfo.flags = 0;
        surfaceCreateInfo.connection = XGetXCBConnection(windowInfo.info.x11.display);
        surfaceCreateInfo.window = windowInfo.info.x11.window;

        vkCreateXcbSurfaceKHR(vulkanContext.getInstance(), &surfaceCreateInfo, nullptr, &surface);

        //TODO CHECK CAPABILITIES
        VkSwapchainCreateInfoKHR swapCreateInfo = {};
        swapCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapCreateInfo.pNext = VK_NULL_HANDLE;
        swapCreateInfo.surface = surface;
        swapCreateInfo.minImageCount = 3;
        swapCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
        swapCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        swapCreateInfo.imageExtent.width = width;
        swapCreateInfo.imageExtent.height = height;
        swapCreateInfo.imageArrayLayers = 1;
        swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
        swapCreateInfo.clipped = VK_TRUE;
        swapCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        swapCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult result = vkCreateSwapchainKHR(vulkanContext.getDevice(), &swapCreateInfo, nullptr, &swapChain);
        if(result != VK_SUCCESS) {
            throw new runtime_error("Could not create swapchain");
        }
    }

    void VulkanDisplay::swap() {
    }

    VulkanDisplay::~VulkanDisplay()
    {
        if(swapChain) {
            vkDestroySwapchainKHR(vulkanContext.getDevice(), swapChain, nullptr);
            swapChain = VK_NULL_HANDLE;
        }
        if(surface) {
            vkDestroySurfaceKHR(vulkanContext.getInstance(), surface, nullptr);
            surface = VK_NULL_HANDLE;
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}
