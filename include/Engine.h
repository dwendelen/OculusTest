#pragma once
#include <memory>




namespace video {
    class Displayy;
    class Camera;
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
public:
	Engine(Displayy& display, Camera& camera, VR& vr);
	void run();
	~Engine();
};

