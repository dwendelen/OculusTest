#pragma once

#include "Camera.h"
#include "Oculus.h"
#include "OculusRenderingTarget.h"
#include <vector>

class OculusCamera: public Camera
{
	ovrSession session;
	Oculus& oculus;
	ovrFovPort fov[2];
	std::vector<RenderingTarget*> renderingTargets;

public:
	OculusCamera::OculusCamera(Oculus& oculus, std::vector<RenderingTarget*> renderingTargets);
	Matrix4f calculatePV(int eye);
	int getNbOfPerspectives() { return 2; }
	RenderingTarget* getRenderingTarget(int perspective);
	~OculusCamera();
};