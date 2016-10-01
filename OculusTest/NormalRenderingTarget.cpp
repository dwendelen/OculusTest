#include "NormalRenderingTarget.h"
#include "GL/glew.h"

NormalRenderingTarget::NormalRenderingTarget(int width, int height):width(width), height(height)
{
	//glViewport(0, 0, width, height);
}

void NormalRenderingTarget::prepareForRendering()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_FRAMEBUFFER_SRGB);
}


NormalRenderingTarget::~NormalRenderingTarget()
{
}
