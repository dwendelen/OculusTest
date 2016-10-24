#pragma once
#include "Camera.h"
#include "VulkanRenderingTarget.h"

using namespace video;

namespace vulkan {
	extern VulkanRenderingTarget target;

    class VulkanCamera : public Camera
    {
    public:
		VulkanCamera(){};
        Matrix4f calculatePV(int perspective);
        int getNbOfPerspectives() { return 1; };
		RenderingTarget& getRenderingTarget(int perspective) { return target; };
        ~VulkanCamera();
    };
}
