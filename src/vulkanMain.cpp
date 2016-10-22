#include "VulkanFwd.h"

#include "VulkanContext.h"
#include "VulkanDisplay.h"
#include "VulkanRenderPass.h"
#include "VulkanCommands.h"
#include "VulkanDescriptors.h"
#include "VulkanMemoryManager.h"
#include "LegoBrick.h"
#include "Scene.h"
#include "VulkanRenderer.h"
#include "Engine.h"
#include "NormalCamera.h"
#include "NullVR.h"
#include "VulkanRenderingTarget.h"

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

    LegoBrick legoBrick;
	legoBrick.init();

	vulkanMemoryManager.load(legoBrick);

    VulkanCommands commands(vulkanContext, vulkanDisplay, renderPass, vulkanDescriptors, vulkanMemoryManager);
    commands.init(legoBrick.getIndices().size());

	Scene scene(legoBrick);
	scene.init();

	

	VulkanRenderer renderer(vulkanContext, vulkanMemoryManager, commands, vulkanDisplay);
	renderer.init();
	//renderer.renderScene(scene, Matrix4f());

	VulkanRenderingTarget renderingTarget;

	DefaultCamera camera(renderingTarget);
	NullVR nullVR;
	Engine engine(vulkanDisplay, camera, nullVR, renderer, legoBrick);
	engine.run();

	vkDeviceWaitIdle(vulkanContext.getDevice());

	return 0;
}
