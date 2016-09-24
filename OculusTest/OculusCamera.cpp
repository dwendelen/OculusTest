#include "OculusCamera.h"

OculusCamera::OculusCamera(Oculus& oculus, std::vector<RenderingTarget*> renderingTargets):oculus(oculus)
{
	this->renderingTargets = renderingTargets;
}

Matrix4f OculusCamera::calculatePV(int eye)
{
	ovrPosef eyePose = oculus.getEyePose(eye);
	Vector3f pos = eyePose.Position;
	Matrix4f rot = Matrix4f(eyePose.Orientation);

	Vector3f finalUp = rot.Transform(Vector3f(0, 1, 0));
	Vector3f finalForward = rot.Transform(Vector3f(0, 0, -1));
	Matrix4f view = Matrix4f::LookAtRH(pos, pos + finalForward, finalUp);

	Matrix4f proj = ovrMatrix4f_Projection(oculus.getFov(eye), 0.1f, 1000.0f, 0);
	return proj * view;
}

RenderingTarget* OculusCamera::getRenderingTarget(int perspective)
{
	return renderingTargets[perspective];
}

OculusCamera::~OculusCamera()
{
}