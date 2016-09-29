#pragma once
#include "Extras/OVR_Math.h"
#include "GL/glew.h"
#include "LegoBrick.h"

using namespace OVR;

class Scene
{
	GLuint program;
	GLuint vertexArray;
	GLuint legoBuffer;
	GLuint legoElements;
	LegoBrick* legoBrick;
	Quatf orientation;
public:
	Scene();
	void init();
	void rotate(Quatf rotation);
	void render(Matrix4f pv);
	~Scene();
};

