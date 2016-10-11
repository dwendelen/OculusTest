#pragma once

#include "Camera.h"
#include <vector>
#include <functional>

using namespace video;

namespace oculus {
	class Oculus;

	class OculusCamera : public Camera
	{
		ovrSession session;
		Oculus& oculus;
		ovrFovPort fov[2];
		std::vector<std::reference_wrapper<RenderingTarget>> renderingTargets;

	public:
		OculusCamera(Oculus& oculus, std::vector<std::reference_wrapper<RenderingTarget>> renderingTargets);
		Matrix4f calculatePV(int eye);
		int getNbOfPerspectives() { return 2; }
		RenderingTarget& getRenderingTarget(int perspective);
		~OculusCamera();
	};
}