#include "Oculus.h"

Oculus::Oculus()
{
}

void Oculus::init()
{
	if (!OVR_SUCCESS(ovr_Initialize(nullptr))) {
		throw "Could not init Oculus";
	}
	if (!OVR_SUCCESS(ovr_Create(&session, &luid))) {
		throw "Could not create session";
	}
	hmdDesc = ovr_GetHmdDesc(session);
	eyeRenderDesc[0] = ovr_GetRenderDesc(session, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	eyeRenderDesc[1] = ovr_GetRenderDesc(session, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

	hmdToEyeViewOffset[0] = eyeRenderDesc[0].HmdToEyeOffset;
	hmdToEyeViewOffset[1] = eyeRenderDesc[1].HmdToEyeOffset;
}

ovrSession Oculus::getSession()
{
	return session;
}

long long Oculus::getFrameNumber()
{
	return frameNumber;
}

void Oculus::beginNewFrame()
{
	frameNumber++;
	double displayMidpointSeconds = ovr_GetPredictedDisplayTime(session, frameNumber);
	ovrTrackingState hmdState = ovr_GetTrackingState(session, displayMidpointSeconds, ovrTrue);
	ovr_CalcEyePoses(hmdState.HeadPose.ThePose, hmdToEyeViewOffset, eyePoses);
}

ovrPosef Oculus::getEyePose(int eye)
{
	return eyePoses[eye];
}

ovrSizei Oculus::getResolution()
{
	return hmdDesc.Resolution;
}

ovrFovPort Oculus::getFov(int eye)
{
	return eyeRenderDesc[eye].Fov;
}

void Oculus::recenter()
{
	ovr_RecenterTrackingOrigin(session);
}

Oculus::~Oculus()
{
	ovr_Destroy(session);
}
