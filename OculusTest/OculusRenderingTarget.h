#pragma once
#include "OVR_CAPI.h"
#include "GL\glew.h"
#include "RenderingTarget.h"
#include "Oculus.h"

class OculusRenderingTarget : public RenderingTarget
{
	Oculus& oculus;
	ovrSession session;
	ovrTextureSwapChain swapChain;
	GLuint depthBuffer;
	GLuint frameBuffer;
	ovrSizei size;
	int eye;
public:
	OculusRenderingTarget(Oculus& oculus) : oculus(oculus) {};
	OculusRenderingTarget(Oculus& oculus, int eye);
	void init();
	ovrTextureSwapChain OculusRenderingTarget::getSwapChain();
	ovrSizei getSize();
	void prepareForRendering();
	void renderingDone();
	~OculusRenderingTarget();
};

