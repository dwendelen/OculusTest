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
    VulkanDisplay::VulkanDisplay(VulkanContext& vulkanContext):
        vulkanContext(vulkanContext),
        surface(VK_NULL_HANDLE),
        window(nullptr)
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
    }

    void VulkanDisplay::swap() {
    }

    VulkanDisplay::~VulkanDisplay()
    {
        if(surface) {
            vkDestroySurfaceKHR(vulkanContext.getInstance(), surface, nullptr);
            surface = VK_NULL_HANDLE;
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}
