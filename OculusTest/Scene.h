#pragma once
#include "Extras/OVR_Math.h"
#include "GL/glew.h"
#include "LegoBrick.h"
#include <memory>
#include "MemoryManager.h"

using namespace OVR;

class Scene
{
	GLuint program;
	std::unique_ptr<LegoBrick> legoBrick;
	Quatf orientation;
	MemoryManager& memoryManager;
public:
	Scene(MemoryManager& memoryManager);
	void init();
	void rotate(Quatf rotation);
	void render(Matrix4f pv);
	~Scene();
};

