#pragma once
class RenderingTarget
{
public:
	RenderingTarget() {}
	virtual void prepareForRendering() {}
	virtual void renderingDone() {}
	~RenderingTarget() {}
};

