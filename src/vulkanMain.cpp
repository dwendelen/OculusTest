#define VK_USE_PLATFORM_XCB_KHR 1

#include "VulkanContext.h"
#include "VulkanDisplay.h"

using namespace std;


using namespace vulkan;

int main(int argc, char* argv[])
{
    VulkanContext VulkanContext;
    VulkanContext.init();

    VulkanDisplay vulkanDisplay(VulkanContext);
    vulkanDisplay.init();
}
