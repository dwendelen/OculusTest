#include "NormalCamera.h"


Matrix4f NormalCamera::calculatePV(int perspective)
{
	Matrix4f view = Matrix4f::LookAtRH(Vector3f(0, 0, 0), Vector3f(0, 0, -1), Vector3f(0, 1, 0));
	Matrix4f proj = Matrix4f::PerspectiveRH(1.5f, 1.0f, 0.1f, 1000.0f);
	return proj * view;
}


NormalCamera::~NormalCamera()
{
}
