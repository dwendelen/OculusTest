#pragma once
#include "Extras/OVR_Math.h"
#include "GL/glew.h"
#include <memory>

class LegoBrick;
class ModelInstance;
class MemoryManager;

using namespace OVR;
using namespace std;

class Scene
{
	GLuint program;
	unique_ptr<LegoBrick> legoBrick;
	unique_ptr<ModelInstance> model;

	Quatf orientation;
	MemoryManager& memoryManager;

public:
	Scene(MemoryManager& memoryManager);
	void init();
	void rotate(Quatf rotation);
	void render(Matrix4f pv);
	void enableWireframe();
	void disableWireframe();
	~Scene();
};

