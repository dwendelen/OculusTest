#pragma once
#include "Renderer.h"

#include "GL/glew.h"

class Model;

namespace memory
{
	class MemoryManager;
}

using namespace memory;

namespace video
{
	class NormalRenderer: public Renderer
	{
	private:
		GLuint program;
		MemoryManager& memoryManager;
		Model& model;
	public:
		NormalRenderer(MemoryManager& memoryManager, Model& model);
		virtual void init();
		virtual void renderScene(Scene& scene, Matrix4f pv);
		virtual ~NormalRenderer();
	};
}