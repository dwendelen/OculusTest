#pragma once
#include "Extras/OVR_Math.h"
#include "GL/glew.h"
#include <memory>
#include <vector>

class ModelInstance;
class Model;

namespace memory {
    class MemoryManager;
}

using namespace OVR;
using namespace std;
using namespace memory;

#include "ModelInstance.h"

class Scene
{
	Model& legoBrick;
	unique_ptr<ModelInstance> model;
	vector<ModelInstance> placedBlocks;

	Quatf orientation;
	int colorIndex;

public:
	Scene(Model& model);
	void init();
	void rotate(Quatf rotation);
	void move(Vector3f translation);
	void moveTo(Vector3f position);
	void place();
	void changeColor();
	void enableWireframe();
	void disableWireframe();
	vector<ModelInstance> getPlacedBlocks() { return placedBlocks; };
	ModelInstance* getCurrentBlock() { return model.get(); };
	~Scene();
};

