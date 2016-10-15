#include "MemoryManager.h"
#include "GL/glew.h"
#include <vector>
#include "Extras/OVR_Math.h"
#include "Model.h"

using namespace std;
using namespace OVR;

namespace memory
{
    MemoryManager::MemoryManager() {}
    void MemoryManager::init()
    {
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glGenBuffers(2, uniformBuffers);

        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffers[0]);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float) * 4 * 4, NULL, GL_STREAM_DRAW);

        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffers[1]);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 4, NULL, GL_STREAM_DRAW);

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBuffers[0]);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, uniformBuffers[1]);
    }

    void MemoryManager::load(const Model& model)
    {
        const vector<Vector3f> verticesAndNormal = model.getVertices();
        glGenBuffers(1, &legoBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, legoBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * verticesAndNormal.size(), verticesAndNormal.data(), GL_STATIC_DRAW);

        const vector<Vector3ui> indices = model.getIndices();
        glGenBuffers(1, &legoElements);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, legoElements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3ui) * indices.size(), indices.data(), GL_STATIC_DRAW);
    }

    void MemoryManager::bindModel() {
        glBindBuffer(GL_ARRAY_BUFFER, legoBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, legoElements);
    }

    void MemoryManager::setUniform(Matrix4f mvp, Matrix4f rot, Vector4f color) {
        struct matr {
            Matrix4f mvp;
            Matrix4f rot;
        };

        struct matr data = {
            mvp.Transposed(),
            rot.Transposed()
        };

        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffers[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);

        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffers[1]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(color), &color);
    }

    MemoryManager::~MemoryManager()
    {
        glDeleteBuffers(1, &legoBuffer);
        glDeleteBuffers(1, &legoElements);
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteBuffers(2, uniformBuffers);
    }
}
