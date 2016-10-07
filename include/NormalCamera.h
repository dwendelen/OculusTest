#pragma once
#include "Camera.h"

class RenderingTarget;

class NormalCamera : public Camera
{
	RenderingTarget& renderingTarget;
public:
	NormalCamera(RenderingTarget& renderingTarget) : renderingTarget(renderingTarget) {
	};
	Matrix4f calculatePV(int perspective);
	int getNbOfPerspectives() { return 1; };
	RenderingTarget& getRenderingTarget(int perspective) { return renderingTarget; };
	~NormalCamera();
};

