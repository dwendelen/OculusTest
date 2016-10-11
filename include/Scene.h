#pragma once
#include "Extras/OVR_Math.h"
#include "GL/glew.h"
#include <memory>
#include <vector>

class LegoBrick;
class ModelInstance;

namespace memory {
    class MemoryManager;
}

using namespace OVR;
using namespace std;
using namespace memory;

class Scene
{
	GLuint program;
	unique_ptr<LegoBrick> legoBrick;
	unique_ptr<ModelInstance> model;
    vector<ModelInstance> placedBlocks;

	Quatf orientation;
	MemoryManager& memoryManager;
	int colorIndex;

public:
	Scene(MemoryManager& memoryManager);
	void init();
	void rotate(Quatf rotation);
	void move(Vector3f translation);
	void moveTo(Vector3f position);
	void place();
	void changeColor();
	void render(Matrix4f pv);
	void enableWireframe();
	void disableWireframe();
	~Scene();
};

