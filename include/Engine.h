#pragma once
#include <memory>

using namespace std;

class Display;
class Camera;
class VR;
class InputManager;

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

