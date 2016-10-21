#pragma once

#include "OVR_CAPI.h"
#include "GL\glew.h"

//#include "Oculus.h"
//#include "OculusRenderingTarget.h"

#include "Display.h"
#include <vector>
#include <memory>

using namespace video;

namespace video {
	class RenderingTarget;
}

namespace oculus {
	class Oculus;
	class OculusRenderingTarget;

	class OculusDisplay : public Displayy
	{
		ovrSession session;
		ovrMirrorTexture mirrorTexture;
		int mirrorTextureWidth;
		int mirrorTextureHeight;
		GLuint mirrorFrameBuffer;
		ovrLayerEyeFov layer;
		Oculus& oculus;
		Displayy* mirrorDisplay;
		std::vector<OculusRenderingTarget> renderingTargets;
	public:
		OculusDisplay(Oculus& oculus, Displayy* mirrorDisplay);
		void init();
		void swap();
		std::vector<std::reference_wrapper<RenderingTarget>> getRenderingTargets();
		void prepareForNewFrame();
		int getWidth();
		int getHeight();
		~OculusDisplay();
	};
}