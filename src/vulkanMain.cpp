#include "VulkanFwd.h"

#include "VulkanContext.h"
#include "VulkanDisplay.h"
#include "VulkanRenderPass.h"
#include "VulkanCommands.h"
#include "VulkanDescriptors.h"
#include "VulkanMemoryManager.h"
#include "LegoBrick.h"

using namespace std;


using namespace vulkan;

int main(int argc, char* argv[])
{
    VulkanContext vulkanContext;
    vulkanContext.init();

    VulkanRenderPass renderPass(vulkanContext, 800, 600);
    renderPass.init();

    VulkanDescriptors vulkanDescriptors(vulkanContext, renderPass);
    vulkanDescriptors.init();

    VulkanMemoryManager vulkanMemoryManager(vulkanContext, vulkanDescriptors);
    vulkanMemoryManager.init();

    VulkanDisplay vulkanDisplay(vulkanContext, renderPass, 800, 600);
    vulkanDisplay.init();

    LegoBrick LegoBrick;

    VulkanCommands commands(vulkanContext, vulkanDisplay, renderPass, vulkanDescriptors, vulkanMemoryManager);
    commands.init(LegoBrick.getIndices().size());

	return 0;
}
