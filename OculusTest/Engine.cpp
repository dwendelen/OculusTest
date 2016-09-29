#include "Engine.h"

#include "SDL/SDL.h"

#include "Scene.h"


Engine::Engine(Oculus* oculus, Display* display, Camera* camera): display(display), camera(camera), oculus(oculus)
{
}

void Engine::run() {
	Scene scene = Scene();
	scene.init();

	float x = 0;
	float y = 0;
	Uint32 lastTicks = 0;

	SDL_Event windowEvent;
	while (true) {
		Uint32 ticks = SDL_GetTicks();
		Uint32 elapsedMilis = ticks - lastTicks;
		lastTicks = ticks;

		while (SDL_PollEvent(&windowEvent))
		{
			switch (windowEvent.type) {
				case SDL_QUIT:
					return;
				case SDL_CONTROLLERBUTTONUP:
					switch (windowEvent.cbutton.button) {
						case SDL_CONTROLLER_BUTTON_BACK:
							if (oculus) {
								ovr_RecenterTrackingOrigin(oculus->getSession());
							}
							break;
						case SDL_CONTROLLER_BUTTON_START:
							return;
					}
					break;
				case SDL_CONTROLLERAXISMOTION:
					switch (windowEvent.caxis.axis) {
					case SDL_CONTROLLER_AXIS_LEFTX:
						if (abs(windowEvent.caxis.value) < 3000) {
							x = 0;
						}
						else {
							x = windowEvent.caxis.value / 32767.0f;
						}
						break;
					case SDL_CONTROLLER_AXIS_LEFTY:
						if (abs(windowEvent.caxis.value) < 3000) {
							y = 0;
						}else {

							y = windowEvent.caxis.value / 32767.0f;
						}
							break;
					}
					break;
			}
		}

		if (oculus) {
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
		}
		
		float radiantsPerMilisec = 0.003f;
		Quatf rotation = Quatf(Vector3f(0, 1, 0), radiantsPerMilisec * elapsedMilis * x);
		rotation *= Quatf(Vector3f(1, 0, 0), radiantsPerMilisec * elapsedMilis * y);
		scene.rotate(rotation);

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
