#pragma once
#include "Renderer.h"

#include "VulkanFwd.h"

using namespace video;

class ModelInstance;

namespace vulkan
{
	class VulkanRenderer: public Renderer
	{
	private:
		VulkanContext& context;
		VulkanMemoryManager& memoryManager;
		VulkanCommands& commands;
		VulkanDisplay& display;

		void submit(const ModelInstance& renderedModel, const Matrix4f& pv, float alpha);
	public:
		VulkanRenderer(VulkanContext& context, VulkanMemoryManager& memoryManager, VulkanCommands& commands, VulkanDisplay& display);
		void init();
		void renderScene(Scene& scene, Matrix4f pv);
		~VulkanRenderer();
	};
}