#pragma once
#include "GL\glew.h"
#include "Extras\OVR_Math.h"
#include <vector>

using namespace OVR;
using namespace std;

typedef Vector3<GLuint>  Vector3ui;

class LegoBrick
{
	vector<Vector3f> vertices;
	vector<Vector3ui> indices;
public:
	LegoBrick();
	void init();
	vector<Vector3f>* getVertices() { return &vertices; }
	vector<Vector3ui>* getIndices() { return &indices; }
	~LegoBrick();
};

