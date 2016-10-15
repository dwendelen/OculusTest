#define VK_USE_PLATFORM_XCB_KHR 1

#include "VulkanContext.h"
#include "VulkanDisplay.h"
#include "VulkanRenderPass.h"

using namespace std;


using namespace vulkan;

int main(int argc, char* argv[])
{
    VulkanContext vulkanContext;
    vulkanContext.init();

    VulkanDisplay vulkanDisplay(vulkanContext, 800, 600);
    vulkanDisplay.init();

    VulkanRenderPass renderPass(vulkanContext, 800, 600);
    renderPass.init();
}
