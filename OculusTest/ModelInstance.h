#pragma once
#include "Model.h"
#include "Extras/OVR_Math.h"

using namespace OVR;

class ModelInstance
{
private:
	const Model& model;
	Vector3f position;
	Quatf orientation;
public:
	ModelInstance(Model& model, Vector3f position, Quatf orientation);
	void rotate(Quatf rotation);
	const Model& getModel() { return model; }
	Matrix4f getModelMatrix();
	Matrix4f getRotationMatrix();
	~ModelInstance();
};