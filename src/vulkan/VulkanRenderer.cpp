#include "VulkanRenderer.h"

#include "ModelInstance.h"
#include "VulkanMemoryManager.h"
#include "VulkanCommands.h"
#include "Scene.h"
#include "VulkanContext.h"
#include "VulkanDisplay.h"

namespace vulkan
{
	VulkanRenderer::VulkanRenderer(VulkanContext& context, VulkanMemoryManager& memoryManager, VulkanCommands& commands, VulkanDisplay& display):
		context(context),
		memoryManager(memoryManager),
		commands(commands),
		display(display)
	{
	}

	void VulkanRenderer::init()
	{
	}

	void VulkanRenderer::submit(const ModelInstance& renderedModel, const Matrix4f& pv, float alpha)
	{
		Matrix4f pvm = pv * renderedModel.getModelMatrix();
		Matrix4f rot = renderedModel.getRotationMatrix();

		memoryManager.setUniform(pvm, rot, Vector4f(renderedModel.getColor(), alpha));

		VkSemaphore imageReady = display.getImageReady();
		VkSemaphore renderingDone = display.getRenderingDone();

		VkCommandBuffer buffer = commands.getCommandBuffer(display.getSwapChainIndex());
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = nullptr;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &buffer;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &imageReady;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &renderingDone;
		info.pWaitDstStageMask = waitStages;

		VkResult r = vkQueueSubmit(context.getQueue(), 1, &info, nullptr);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not submit command buffer");
		}
	}

	void VulkanRenderer::renderScene(Scene& scene, Matrix4f pv)
	{
		for (ModelInstance renderedModel : scene.getPlacedBlocks()) {
			//submit(renderedModel, pv, 1);
		}

		//submit(*scene.getCurrentBlock(), pv, 0);
		submit(*scene.getCurrentBlock(), pv, 0.5f);
	}


	VulkanRenderer::~VulkanRenderer()
	{
	}
}