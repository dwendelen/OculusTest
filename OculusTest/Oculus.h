#pragma once
#include "OVR_CAPI.h"
#include "VR.h"

class Oculus: public VR
{
	ovrSession session;
	ovrGraphicsLuid luid;
	long long frameNumber = 0;
	ovrHmdDesc hmdDesc;
	ovrPosef eyePoses[2];
	ovrVector3f hmdToEyeViewOffset[2];
	ovrEyeRenderDesc eyeRenderDesc[2];
public:
	Oculus();
	void init();
	ovrSession getSession();
	ovrFovPort getFov(int eye);
	void beginNewFrame();
	long long getFrameNumber();
	ovrPosef getEyePose(int eye);
	ovrSizei getResolution();
	virtual void recenter();
	~Oculus();
};

