#pragma once

#include "Extras/OVR_Math.h"

using namespace OVR;

class Scene;

namespace video
{
	class Renderer
	{
	public:
		virtual void init() = 0;
		virtual void renderScene(Scene& scene, Matrix4f pv) = 0;
		virtual ~Renderer() {};
	};
}