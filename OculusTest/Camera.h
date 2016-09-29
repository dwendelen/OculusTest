#pragma once
#include "Extras/OVR_Math.h"
#include "RenderingTarget.h"
using namespace OVR;

class Camera
{
public:
	virtual Matrix4f calculatePV(int perspective) = 0;
	virtual int getNbOfPerspectives() = 0;
	virtual RenderingTarget* getRenderingTarget(int perspective) = 0;
	virtual ~Camera() {}
};
