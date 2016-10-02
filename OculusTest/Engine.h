#pragma once
#include "Display.h"
#include "Camera.h"
//#include "Oculus.h"

class Engine
{
	Display& display;
	Camera& camera;
	//Oculus* oculus;
public:
	Engine(/*Oculus* oculus,*/ Display& display, Camera& camera);
	void run();
	~Engine();
};

