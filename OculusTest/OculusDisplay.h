#pragma once

#include "OVR_CAPI.h"

#include "Oculus.h"
#include "OculusRenderingTarget.h"
#include "Display.h"
#include <vector>
#include <memory>

class OculusDisplay: public Display
{
	ovrSession session;
	ovrMirrorTexture mirrorTexture;
	int mirrorTextureWidth;
	int mirrorTextureHeight;
	GLuint mirrorFrameBuffer;
	ovrLayerEyeFov layer;
	Oculus& oculus;
	Display* mirrorDisplay;
	std::unique_ptr<OculusRenderingTarget> renderingTargets[2];
public:
	OculusDisplay(Oculus& oculus, Display* mirrorDisplay);
	void init();
	void swap();
	std::vector<RenderingTarget*> getRenderingTargets() {
		std::vector<RenderingTarget*> result = std::vector<RenderingTarget*>();
		result.push_back(renderingTargets[0].get());
		result.push_back(renderingTargets[1].get());
		return result;
	}
	void prepareForNewFrame();
	int getWidth();
	int getHeight();
	~OculusDisplay();
};