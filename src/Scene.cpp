#include "Scene.h"
#include "LegoBrick.h"
#include "ModelInstance.h"
#include "MemoryManager.h"

using namespace OVR;
using namespace memory;
using namespace std;

const float GRID_X = 0.0080f;
const float GRID_Y = 0.0096f;
const float GRID_Z = 0.0080f;

const float HALF_GRID_X = GRID_X / 2;
const float HALF_GRID_Y = GRID_Y / 2;
const float HALF_GRID_Z = GRID_Z / 2;

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
void Scene::moveUnit(int x, int y, int z) {
    move(GRID_X * x, GRID_Y * y, GRID_Z * z);
}
float calculateMovement(float& value, float delta, float halfGrid, float grid) {
    value += delta;
    if(value > 0) {
        if(value <= halfGrid) {
            return 0.0f;
        } else {
            value = value - grid;
            return grid;
        }
    } else {
        if(value >= -halfGrid) {
            return 0.0f;
        } else {
            value = value + grid;
            return -grid;
        }
    }
}
void Scene::move(float x, float y, float z) {
    Vector3f movement;
    movement.x = calculateMovement(pseudoPosition.x, x, HALF_GRID_X, GRID_X);
    movement.y = calculateMovement(pseudoPosition.y, y, HALF_GRID_Y, GRID_Y);
    movement.z = calculateMovement(pseudoPosition.z, z, HALF_GRID_Z, GRID_Z);
    model->move(movement);

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
