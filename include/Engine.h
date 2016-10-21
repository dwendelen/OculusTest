#pragma once
#include <memory>


class Model;

namespace video {
    class Displayy;
    class Camera;
	class Renderer;
}
class VR;
namespace input {
    class InputManager;
}

using namespace std;
using namespace video;
using namespace input;

class Engine
{
	Displayy& display;
	Camera& camera;
	VR& vr;
	unique_ptr<InputManager> inputManager;
	Renderer& renderer;
	Model& model;
public:
	Engine(Displayy& display, Camera& camera, VR& vr, Renderer& renderer, Model& model);
	void run();
	~Engine();
};

