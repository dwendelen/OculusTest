#pragma once
#include <memory>




namespace video {
    class Display;
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
	Display& display;
	Camera& camera;
	VR& vr;
	unique_ptr<InputManager> inputManager;
public:
	Engine(Display& display, Camera& camera, VR& vr);
	void run();
	~Engine();
};

