#include "Engine.h"
#include "SDL.h"
#include "Scene.h"
#include "MemoryManager.h"

Engine::Engine(Display& display, Camera& camera):
    display(display),
    camera(camera)
{
}

void Engine::run() {
    MemoryManager loader;
    loader.init();
	Scene scene(loader);
	scene.init();

	inputManager.reset(new InputManager(scene));
    inputManager->init();

	while (true) {
        inputManager->processInput();
        if(inputManager->isShouldQuit()) {
            return;
        }
		display.prepareForNewFrame();
		for (int perspective = 0; perspective < camera.getNbOfPerspectives(); perspective++) {
			Matrix4f pv = camera.calculatePV(perspective);
			RenderingTarget& renderingTarget = camera.getRenderingTarget(perspective);
			renderingTarget.prepareForRendering();
			scene.render(pv);
			renderingTarget.renderingDone();
		}
		display.swap();
	}
}

Engine::~Engine()
{
}
