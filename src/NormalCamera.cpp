#include "DefaultCamera.h"

namespace video {
    Matrix4f DefaultCamera::calculatePV(int perspective)
    {
        Matrix4f view = Matrix4f::LookAtRH(Vector3f(0, 0, 0), Vector3f(0, 0, -1), Vector3f(0, 1, 0));
        Matrix4f proj = Matrix4f::PerspectiveRH(0.3f, 4.0f/3.0f, 0.05f, 10.0f);
        return proj * view;
    }


    DefaultCamera::~DefaultCamera()
    {
    }
}
