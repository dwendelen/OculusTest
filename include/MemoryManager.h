#pragma once

#include "GL/glew.h"

class Model;

namespace memory {
    class MemoryManager
    {
        private:
            GLuint vertexArray;
            GLuint legoBuffer;
            GLuint legoElements;
        public:
            MemoryManager();
            void init();
            void load(const Model& model);
            void bindModel();
            ~MemoryManager();
    };
}
