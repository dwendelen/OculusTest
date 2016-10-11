#pragma once
#include "Extras/OVR_Math.h"

using namespace OVR;

namespace video {
    class RenderingTarget;

    class Camera
    {
    public:
        virtual Matrix4f calculatePV(int perspective) = 0;
        virtual int getNbOfPerspectives() = 0;
        virtual RenderingTarget& getRenderingTarget(int perspective) = 0;
        virtual ~Camera() {}
    };
}
