#pragma once
#include "Camera.h"

namespace video {
    class RenderingTarget;

    class DefaultCamera : public Camera
    {
        RenderingTarget& renderingTarget;
    public:
        DefaultCamera(RenderingTarget& renderingTarget) : renderingTarget(renderingTarget) {
        };
        Matrix4f calculatePV(int perspective);
        int getNbOfPerspectives() { return 1; };
        RenderingTarget& getRenderingTarget(int perspective) { return renderingTarget; };
        ~DefaultCamera();
    };
}
