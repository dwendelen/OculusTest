#include "OculusDisplay.h"

#include "GL/glew.h"
#include "OVR_CAPI_GL.h"
#include "Extras/OVR_Math.h"

OculusDisplay::OculusDisplay(Oculus& oculus, Display* mirrorDisplay):
	oculus(oculus), mirrorDisplay(mirrorDisplay)
{
	this->session = oculus.getSession();
	this->mirrorTextureHeight = mirrorDisplay->getHeight();
	this->mirrorTextureWidth = mirrorDisplay->getWidth();
}

void OculusDisplay::init()
{
	ovrMirrorTextureDesc desc;
	desc.MiscFlags = ovrTextureMisc_None;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.Width = this->mirrorTextureWidth;
	desc.Height = this->mirrorTextureHeight;
	ovr_CreateMirrorTextureGL(session, &desc, &mirrorTexture);
	GLuint texId;
	ovr_GetMirrorTextureBufferGL(session, mirrorTexture, &texId);

	glGenFramebuffers(1, &mirrorFrameBuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFrameBuffer);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	for (int eye = 0; eye < 2; eye++) {
		renderingTargets.push_back(OculusRenderingTarget(oculus, eye));
		renderingTargets[eye].init();
		layer.ColorTexture[eye] = renderingTargets[eye].getSwapChain();
		layer.Fov[eye] = oculus.getFov(eye);
		layer.Viewport[eye] = OVR::Recti(renderingTargets[eye].getSize());
	}
	layer.Header.Type = ovrLayerType_EyeFov;
	layer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;
}

void OculusDisplay::swap()
{
	ovrLayerHeader* layers = &layer.Header;
	ovr_SubmitFrame(session, oculus.getFrameNumber(), nullptr, &layers, 1);

	mirrorDisplay->prepareForNewFrame();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, mirrorTextureHeight, mirrorTextureWidth, 0,
		0, 0, mirrorTextureWidth, mirrorTextureHeight,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	mirrorDisplay->swap();
}

void OculusDisplay::prepareForNewFrame()
{
	oculus.beginNewFrame();
	layer.RenderPose[0] = oculus.getEyePose(0);
	layer.RenderPose[1] = oculus.getEyePose(1);
}

int OculusDisplay::getWidth()
{
	return oculus.getResolution().w;
}

int OculusDisplay::getHeight()
{
	return oculus.getResolution().h;
}

OculusDisplay::~OculusDisplay()
{
	ovr_DestroyMirrorTexture(session, mirrorTexture);
	glDeleteFramebuffers(1, &mirrorFrameBuffer);
}
