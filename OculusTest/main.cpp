#include "Extras\OVR_Math.h"
#include <memory>
#include "SDL\SDL.h"

#include "Oculus.h"
#include "OculusCamera.h"
#include "OculusDisplay.h"

#include "NormalCamera.h"
#include "NormalDisplay.h"

#include "Scene.h"

#include <exception>
#include <vector>

using namespace OVR;

int WinMain() {
	bool useOculus = true;
	int width = 800;
	int height = 600;

	std::unique_ptr<NormalDisplay> normalDisplay = std::unique_ptr<NormalDisplay>(new NormalDisplay(width, height));
	normalDisplay->init();

	std::unique_ptr<Display> display;
	std::unique_ptr<Camera> camera;
	Oculus oculus = Oculus();

	if (useOculus) {
		oculus.init();

		int width = oculus.getResolution().w / 2;
		int height = oculus.getResolution().h / 2;

		OculusDisplay* disp = new OculusDisplay(oculus, normalDisplay.get());
		disp->init();

		display = std::unique_ptr<Display>(disp);

		camera = std::unique_ptr<Camera>(new OculusCamera(oculus, disp->getRenderingTargets()));
	}
	else {
		camera = std::unique_ptr<Camera>(new NormalCamera(normalDisplay->getRenderingTarget()));
		display = std::unique_ptr<Display>(std::move(normalDisplay));
	}
	
	Scene scene = Scene();
	scene.init();

	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
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

	return 0;
}