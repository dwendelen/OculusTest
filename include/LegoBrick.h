#pragma once
#include "Model.h"

using namespace OVR;
using namespace std;


class LegoBrick: public Model
{
	vector<Vector3f> vertices;
	vector<Vector3ui> indices;
public:
	LegoBrick();
	void init();
	virtual const vector<Vector3f> getVertices() const { return vertices; }
	virtual const vector<Vector3ui> getIndices() const { return indices; }
	~LegoBrick();
};

