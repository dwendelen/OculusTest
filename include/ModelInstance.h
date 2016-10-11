#pragma once
#include "Extras/OVR_Math.h"

using namespace OVR;

class Model;

class ModelInstance
{
private:
	const Model& model;
	Vector3f position;
	Quatf orientation;
public:
	ModelInstance(Model& model, Vector3f position, Quatf orientation);
	void rotate(Quatf rotation);
	void move(Vector3f translation);
	const Model& getModel() { return model; }
	Matrix4f getModelMatrix() const;
	Matrix4f getRotationMatrix() const;
	~ModelInstance();
};
