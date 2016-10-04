#pragma once
#include "Model.h"

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
