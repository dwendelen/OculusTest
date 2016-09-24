#pragma once
#include "Extras\OVR_Math.h"
#include "GL\glew.h"
#include "LegoBrick.h"

using namespace OVR;

class Scene
{
	GLuint program;
	GLuint vertexArray;
	GLuint vertexbuffer;
	GLuint legoBuffer;
	GLuint legoElements;
	LegoBrick* legoBrick;
public:
	Scene();
	void init();
	void render(Matrix4f pv);
	~Scene();
};

