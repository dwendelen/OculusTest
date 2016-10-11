#pragma once
#include "OVR_CAPI.h"
#include "GL/glew.h"
#include "RenderingTarget.h"

using namespace video;

namespace oculus {
	class Oculus;
	class OculusRenderingTarget : public RenderingTarget
	{
		Oculus& oculus;
		ovrSession session;
		ovrTextureSwapChain swapChain;
		GLuint depthBuffer;
		GLuint frameBuffer;
		ovrSizei size;
		int eye;
		bool alive;
	public:
		OculusRenderingTarget(Oculus& oculus) : oculus(oculus) {};
		OculusRenderingTarget(Oculus& oculus, int eye);
		OculusRenderingTarget(OculusRenderingTarget && moveFom);
		void init();
		ovrTextureSwapChain getSwapChain();
		ovrSizei getSize();
		void prepareForRendering();
		void renderingDone();
		~OculusRenderingTarget();
	};
}
