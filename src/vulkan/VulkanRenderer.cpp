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
		display(display),
		firstObject(VK_NULL_HANDLE),
		betweenObjects(VK_NULL_HANDLE),
		afterObjects(VK_NULL_HANDLE)
	{
	}

	void VulkanRenderer::init()
	{
		firstObject = createSemaphore();
		betweenObjects = createSemaphore();
		afterObjects = createSemaphore();
		display.setSemaphores(firstObject, afterObjects);
	}

	void VulkanRenderer::submit(const ModelInstance& renderedModel, const Matrix4f& pv, float alpha, VkSemaphore wait, VkSemaphore signal)
	{
		Matrix4f pvm = pv * renderedModel.getModelMatrix();
		Matrix4f rot = renderedModel.getRotationMatrix();

		memoryManager.setUniform(pvm, rot, Vector4f(renderedModel.getColor(), alpha));

		VkCommandBuffer buffer = commands.getCommandBuffer(display.getSwapChainIndex());
		VkPipelineStageFlags waitStages[] = { /*VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT*/ VK_PIPELINE_STAGE_VERTEX_INPUT_BIT };
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = nullptr;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &buffer;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &wait;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &signal;
		info.pWaitDstStageMask = waitStages;

		VkResult r = vkQueueSubmit(context.getQueue(), 1, &info, nullptr);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not submit command buffer");
		}
	}

	VkSemaphore VulkanRenderer::createSemaphore()
	{
		VkSemaphore sema;
		VkSemaphoreCreateInfo semaInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, 0, 0 };
		VkResult r = vkCreateSemaphore(context.getDevice(), &semaInfo, nullptr, &sema);
		if (r != VK_SUCCESS) {
			throw new runtime_error("Could not create image ready semaphore");
		}
		return sema;
	}

	void VulkanRenderer::renderScene(Scene& scene, Matrix4f pv)
	{
		for (ModelInstance renderedModel : scene.getPlacedBlocks()) {
			//submit(renderedModel, pv, 1);
		}

		submit(*scene.getCurrentBlock(), pv, 1, firstObject, betweenObjects);
		submit(*scene.getCurrentBlock(), pv, 0.5f, betweenObjects, afterObjects);
	}


	VulkanRenderer::~VulkanRenderer()
	{
		vkDestroySemaphore(context.getDevice(), firstObject, nullptr);
		vkDestroySemaphore(context.getDevice(), betweenObjects, nullptr);
		vkDestroySemaphore(context.getDevice(), afterObjects, nullptr);
	}
}