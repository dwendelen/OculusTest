#include "Engine.h"
#include "SDL.h"
#include "Scene.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "Display.h"
#include "Camera.h"
#include "RenderingTarget.h"

Engine::Engine(Displayy& display, Camera& camera, VR& vr):
    display(display),
    camera(camera),
    vr(vr)
{
}

void Engine::run() {
    MemoryManager loader;
    loader.init();
	Scene scene(loader);
	scene.init();

	inputManager.reset(new InputManager(scene, vr));
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
