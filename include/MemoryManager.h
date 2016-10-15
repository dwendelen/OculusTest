#pragma once

#include "GL/glew.h"
#include "Extras/OVR_Math.h"

class Model;
using namespace OVR;

namespace memory {
    class MemoryManager
    {
        private:
            GLuint vertexArray;
            GLuint legoBuffer;
            GLuint legoElements;
            GLuint uniformBuffers[2];
        public:
            MemoryManager();
            void init();
            void load(const Model& model);
            void setUniform(Matrix4f mvp, Matrix4f rot, Vector4f color);
            void bindModel();
            ~MemoryManager();
    };
}
