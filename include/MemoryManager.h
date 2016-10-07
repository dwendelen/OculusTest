#pragma once

#include "GL/glew.h"

class Model;

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
