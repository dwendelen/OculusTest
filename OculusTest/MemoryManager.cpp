#include "MemoryManager.h"


MemoryManager::MemoryManager()
{}
void MemoryManager::init()
{
    glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
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
MemoryManager::~MemoryManager()
{
    glDeleteBuffers(1, &legoBuffer);
	glDeleteBuffers(1, &legoElements);
	glDeleteVertexArrays(1, &vertexArray);
}
