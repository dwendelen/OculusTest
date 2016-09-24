#include "Engine.h"

#include "SDL\SDL.h"

#include "Scene.h"


Engine::Engine(Oculus* oculus, Display* display, Camera* camera): display(display), camera(camera), oculus(oculus)
{
}

void Engine::run() {
	Scene scene = Scene();
	scene.init();

	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
				break;
			if (windowEvent.type == SDL_CONTROLLERBUTTONUP) {
				if (windowEvent.cbutton.button == SDL_CONTROLLER_BUTTON_BACK) {
					ovr_RecenterTrackingOrigin(oculus->getSession());
				}
			}
		}
		ovrSessionStatus sessionStatus;

		ovr_GetSessionStatus(oculus->getSession(), &sessionStatus);
		if (sessionStatus.ShouldQuit)
			break;
		if (sessionStatus.ShouldRecenter)
			ovr_RecenterTrackingOrigin(oculus->getSession());
		ovrInputState input;
		ovr_GetInputState(oculus->getSession(), ovrControllerType_XBox, &input);
		if (input.Buttons & ovrButton_Back) {
			input = ovrInputState();
		}

		display->prepareForNewFrame();
		for (int perspective = 0; perspective < camera->getNbOfPerspectives(); perspective++) {
			Matrix4f pv = camera->calculatePV(perspective);
			RenderingTarget* renderingTarget = camera->getRenderingTarget(perspective);
			renderingTarget->prepareForRendering();
			scene.render(pv);
			renderingTarget->renderingDone();
		}
		display->swap();
	}
}

Engine::~Engine()
{
}
