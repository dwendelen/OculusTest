#pragma once
#include "Display.h"
#include "Camera.h"
#include "InputManager.h"
#include <memory>

using namespace std;

class Engine
{
	Display& display;
	Camera& camera;
	unique_ptr<InputManager> inputManager;
public:
	Engine(Display& display, Camera& camera);
	void run();
	~Engine();
};

