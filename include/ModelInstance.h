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
	Vector3f color;
public:
	ModelInstance(Model& model, Vector3f position, Quatf orientation);
	void rotate(Quatf rotation);
	void move(Vector3f translation);
	void setColor(Vector3f color) {
        this->color = color;
	}
	Vector3f getColor() const {
        return color;
	}
	const Model& getModel() const { return model; }
	Matrix4f getModelMatrix() const;
	Matrix4f getRotationMatrix() const;
	~ModelInstance();
};
