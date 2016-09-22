#pragma once
#include "RenderingTarget.h"
class NormalRenderingTarget :
	public RenderingTarget
{
	int width;
	int height;
public:
	NormalRenderingTarget(int width, int height);
	void prepareForRendering();
	~NormalRenderingTarget();
};

