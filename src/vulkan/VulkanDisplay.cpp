#include "VulkanDisplay.h"

#include "VulkanContext.h"
#include "VulkanRenderPass.h"
#include "VulkanMemoryManager.h"

#include "SDL_syswm.h"
#ifdef __LINUX__
	#include <X11/Xlib-xcb.h>
#endif
#include <stdexcept>
#include <string>

#include "iostream"

using namespace std;

namespace vulkan
{
    VulkanDisplay::VulkanDisplay(VulkanContext& vulkanContext, VulkanRenderPass& renderPass, VulkanMemoryManager& memoryManage, int width, int height):
        vulkanContext(vulkanContext),
        renderPass(renderPass),
		memoryManager(memoryManage),
        surface(VK_NULL_HANDLE),
        swapChain(VK_NULL_HANDLE),
        window(nullptr),
        height(height),
        width(width)
    {}

    void VulkanDisplay::init() {
		VkResult r;
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
		#ifdef __LINUX__
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

			r = vkCreateXcbSurfaceKHR(vulkanContext.getInstance(), &surfaceCreateInfo, nullptr, &surface);
			if (r != VK_SUCCESS) {
				throw new runtime_error("Could not create surface");
			}
		#endif
		#ifdef __WINDOWS__
			if(windowInfo.subsystem != SDL_SYSWM_WINDOWS) {
				throw new runtime_error("Not Windows");
			}

			PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>
				(vkGetInstanceProcAddr(vulkanContext.getInstance(), "vkCreateWin32SurfaceKHR"));

			/*TCHAR* className;
			GetClassName(windowInfo.info.win.window, className, 256);
			WNDCLASS wce;
			GetClassInfo(GetModuleHandle(NULL), className, &wce);
			GetModuleH*/
			VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.pNext = nullptr;
			surfaceCreateInfo.flags = 0;
			surfaceCreateInfo.hwnd = windowInfo.info.win.window;
			surfaceCreateInfo.hinstance = GetModuleHandle(NULL);

			r = vkCreateWin32SurfaceKHR(vulkanContext.getInstance(), &surfaceCreateInfo, nullptr, &surface);
			if (r != VK_SUCCESS) {
				throw new runtime_error("Could not create surface");
			}
		#endif
        //TODO CHECK CAPABILITIES
        VkSwapchainCreateInfoKHR swapCreateInfo = {};
        swapCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapCreateInfo.pNext = VK_NULL_HANDLE;
		swapCreateInfo.flags = 0;
        swapCreateInfo.surface = surface;
        swapCreateInfo.minImageCount = 3;
        swapCreateInfo.imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
        swapCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        swapCreateInfo.imageExtent.width = width;
        swapCreateInfo.imageExtent.height = height;
        swapCreateInfo.imageArrayLayers = 1;
        swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapCreateInfo.queueFamilyIndexCount = 0;
		swapCreateInfo.pQueueFamilyIndices = nullptr;
        swapCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
        swapCreateInfo.clipped = VK_TRUE;
        swapCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        swapCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult result = vkCreateSwapchainKHR(vulkanContext.getDevice(), &swapCreateInfo, nullptr, &swapChain);
        if(result != VK_SUCCESS) {
            throw new runtime_error("Could not create swapchain");
        }

		memoryManager.createDepthBuffer(800, 600);
		VkImageView depthView = memoryManager.getDepthView();

        vector<VkImage> images = getVector(vkGetSwapchainImagesKHR, vulkanContext.getDevice(), swapChain, "Could not get swapChain images");
        for(VkImage image: images) {
            VkImageView view;
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.pNext = nullptr;
            viewInfo.flags = 0;
            viewInfo.image = image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
            viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            result = vkCreateImageView(vulkanContext.getDevice(), &viewInfo, nullptr, &view);
            if(result != VK_SUCCESS) {
                throw new runtime_error("Could not create image views");
            }
            views.push_back(view);

			
            VkImageView attachments[] = {view, depthView};

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass.getRenderPass();
            framebufferInfo.attachmentCount = 2;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = width;
            framebufferInfo.height = height;
            framebufferInfo.layers = 1;

            VkFramebuffer framebuffer;
            result = vkCreateFramebuffer(vulkanContext.getDevice(), &framebufferInfo, nullptr, &framebuffer);
            if (result != VK_SUCCESS) {
                throw runtime_error("Could not create framebuffer");
            }
            framebuffers.push_back(framebuffer);


        }
    }

	void VulkanDisplay::prepareForNewFrame()
	{
		VkResult r = vkAcquireNextImageKHR(vulkanContext.getDevice(), swapChain, 30000000000, imageReady, VK_NULL_HANDLE, &swapIndex);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not aquire next image in swapChain");
		}
	}
    
	void VulkanDisplay::swap() {
		VkResult r;
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderingDone;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChain;
		presentInfo.pImageIndices = &swapIndex;
		presentInfo.pResults = &r;

		vkQueuePresentKHR(vulkanContext.getQueue(), &presentInfo);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not swap swapchain");
		}
		SDL_Delay(10);
    }

    VulkanDisplay::~VulkanDisplay()
    {
		if (imageReady) {
			vkDestroySemaphore(vulkanContext.getDevice(), imageReady, nullptr);
			imageReady = VK_NULL_HANDLE;
		}

		if (renderingDone) {
			vkDestroySemaphore(vulkanContext.getDevice(), renderingDone, nullptr);
			renderingDone = VK_NULL_HANDLE;
		}

        for(VkFramebuffer framebuffer: framebuffers) {
            vkDestroyFramebuffer(vulkanContext.getDevice(), framebuffer, nullptr);
        }
        framebuffers.clear();

        for(VkImageView view: views) {
            vkDestroyImageView(vulkanContext.getDevice(), view, nullptr);
        }
        views.clear();

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
