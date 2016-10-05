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
	std::vector<OculusRenderingTarget> renderingTargets;
public:
	OculusDisplay(Oculus& oculus, Display* mirrorDisplay);
	void init();
	void swap();
	std::vector<std::reference_wrapper<RenderingTarget>> getRenderingTargets() {
		std::vector<std::reference_wrapper<RenderingTarget>> result;
		result.push_back(renderingTargets[0]);
		result.push_back(renderingTargets[1]);
		return result;
		//return renderingTargets;
	}
	void prepareForNewFrame();
	int getWidth();
	int getHeight();
	~OculusDisplay();
};
