#pragma once
#include "Extras\OVR_Math.h"
#include "GL\glew.h"

using namespace OVR;

class Scene
{
	GLuint program;
	GLuint vertexArray;
	GLuint vertexbuffer;

public:
	Scene();
	void init();
	void render(Matrix4f pv);
	~Scene();
};

