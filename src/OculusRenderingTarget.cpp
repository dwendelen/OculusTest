#include "OculusRenderingTarget.h"
#include "OVR_CAPI_GL.h"
#include "Oculus.h"

namespace oculus {

	OculusRenderingTarget::OculusRenderingTarget(Oculus& oculus, int eye) :
		oculus(oculus),
		eye(eye),
		alive(true)
	{
		this->session = oculus.getSession();
	}

	OculusRenderingTarget::OculusRenderingTarget(OculusRenderingTarget&& moveFom) :
		oculus(moveFom.oculus),
		eye(moveFom.eye),
		session(moveFom.session),
		alive(true),
		swapChain(moveFom.swapChain),
		depthBuffer(moveFom.depthBuffer),
		frameBuffer(moveFom.frameBuffer),
		size(moveFom.size)
	{
		moveFom.alive = false;
	}


	void OculusRenderingTarget::init() {
		size = ovr_GetFovTextureSize(session, ovrEyeType(eye), oculus.getFov(eye), 1.0f);

		ovrTextureSwapChainDesc desc = {};
		desc.Type = ovrTexture_2D;
		desc.ArraySize = 1;
		desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.Width = size.w;
		desc.Height = size.h;
		desc.MipLevels = 1;
		desc.SampleCount = 1;
		desc.StaticImage = ovrFalse;

		ovr_CreateTextureSwapChainGL(session, &desc, &swapChain);

		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.w, size.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		int length;
		ovr_GetTextureSwapChainLength(session, swapChain, &length);

		for (int i = 0; i < length; i++) {
			GLuint texture;
			ovr_GetTextureSwapChainBufferGL(session, swapChain, i, &texture);

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	}

	void OculusRenderingTarget::prepareForRendering()
	{
		GLuint texture;
		int textureIndex;
		ovr_GetTextureSwapChainCurrentIndex(session, swapChain, &textureIndex);
		ovr_GetTextureSwapChainBufferGL(session, swapChain, textureIndex, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glViewport(0, 0, size.w, size.h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_FRAMEBUFFER_SRGB);
	}

	ovrTextureSwapChain OculusRenderingTarget::getSwapChain() {
		return swapChain;
	}

	ovrSizei OculusRenderingTarget::getSize() {
		return size;
	}

	void OculusRenderingTarget::renderingDone()
	{
		ovr_CommitTextureSwapChain(session, swapChain);
	}

	OculusRenderingTarget::~OculusRenderingTarget()
	{
		if (alive) {
			ovr_DestroyTextureSwapChain(session, swapChain);
			glDeleteRenderbuffers(1, &depthBuffer);
			glDeleteFramebuffers(1, &frameBuffer);
		}
	}
}