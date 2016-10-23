#include "VulkanCamera.h"

namespace vulkan {
	VulkanRenderingTarget target;

    Matrix4f VulkanCamera::calculatePV(int perspective)
    {
		Vector3f z = Vector3f(0.0f,  0.0f, -1.0f);  // Forward
		Vector3f x = Vector3f(1.0f,  0.0f,  0.0f); // Right
		Vector3f y = Vector3f(0.0f, -1.0f,  0.0f);

		Matrix4f view(	x.x, x.y, x.z, 0,
						y.x, y.y, y.z, 0,
						z.x, z.y, z.z, 0,
						0  , 0  , 0  , 1);
		Matrix4f proj;
		float tanHalfFov = tan(0.15f);

		proj.M[0][0] = 0.75 / tanHalfFov;
		proj.M[1][1] = 1 / tanHalfFov;
		proj.M[2][2] = 10.0f / (10.0f - 0.05f);
		proj.M[2][3] = (0.05f * 10.0f) / (0.05f - 10.0f);
		proj.M[3][2] = 1;
		proj.M[3][3] = 0;
		return proj * view;
    }

	VulkanCamera::~VulkanCamera()
    {
    }
}
