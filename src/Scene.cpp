#include "Scene.h"
#include "LegoBrick.h"
#include "ModelInstance.h"
#include "MemoryManager.h"

using namespace OVR;
using namespace memory;
using namespace std;


Scene::Scene(Model& model):
    legoBrick(model),
    colorIndex(0)
{
}

const vector<Vector3f> colors = {
    Vector3f(1.0f, 1.0f, 1.0f),
    Vector3f(1.0f, 0.0f, 0.0f),
    Vector3f(0.0f, 1.0f, 0.0f),
    Vector3f(0.0f, 0.0f, 1.0f)
};

void Scene::init() {
	orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
	model.reset(new ModelInstance(legoBrick, Vector3f(0.0f, 0.0f, -0.2f), orientation));
    model->setColor(colors[0]);
}

void Scene::rotate(Quatf rotation)
{
	model->rotate(rotation);
}

void Scene::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
}

void Scene::disableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void Scene::move(Vector3f translation){
    model->move(translation);
}
void Scene::moveTo(Vector3f position) {}

void Scene::changeColor() {
    colorIndex = (colorIndex + 1) % colors.size();
    model->setColor(colors[colorIndex]);
}

void Scene::place() {
    placedBlocks.push_back(*model);
}

Scene::~Scene()
{

}