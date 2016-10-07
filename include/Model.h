#pragma once
#include <vector>
#include "Extras/OVR_Math.h"
#include "GL/glew.h"

using namespace std;
using namespace OVR;

typedef Vector3<GLuint>  Vector3ui;

class Model
{
    public:
        virtual const vector<Vector3f> getVertices() const = 0;
        virtual const vector<Vector3ui> getIndices() const = 0;
};
