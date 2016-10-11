#include "ModelInstance.h"

ModelInstance::ModelInstance(Model& model, Vector3f position, Quatf orientation) :
	model(model),
	orientation(orientation),
	position(position)
{
}

void ModelInstance::rotate(Quatf rotation)
{
	orientation = rotation * orientation;
	if (!orientation.IsNormalized()) {
		orientation.Normalize();
	}
}

void ModelInstance::move(Vector3f translation) {
    position += translation;
}

Matrix4f ModelInstance::getModelMatrix() const
{
	return Matrix4f::Translation(position) * Matrix4f(orientation);
}

Matrix4f ModelInstance::getRotationMatrix() const
{
	return Matrix4f(orientation);
}

ModelInstance::~ModelInstance()
{

}
