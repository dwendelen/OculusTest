#pragma once
#include "Model.h"

class MemoryManager
{
    private:
        GLuint vertexArray;
        GLuint legoBuffer;
        GLuint legoElements;
        GLuint legoLineElements;
    public:
        MemoryManager();
        void init();
        void load(const Model& model);
        void bindModel();
        ~MemoryManager();
};
