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

Matrix4f ModelInstance::getModelMatrix()
{
	return Matrix4f::Translation(position) * Matrix4f(orientation);
}

Matrix4f ModelInstance::getRotationMatrix()
{
	return Matrix4f(orientation);
}

ModelInstance::~ModelInstance()
{

}