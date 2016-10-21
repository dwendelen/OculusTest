#include "Engine.h"
#include "SDL.h"
#include "Scene.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "Display.h"
#include "Camera.h"
#include "RenderingTarget.h"
#include "Renderer.h"

Engine::Engine(Displayy& display, Camera& camera, VR& vr, Renderer& renderer, Model& model):
    display(display),
    camera(camera),
    vr(vr),
	renderer(renderer),
	model(model)
{
}

void Engine::run() {
	Scene scene(model);
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
			renderer.renderScene(scene, pv);
			renderingTarget.renderingDone();
		}
		display.swap();
	}
}

Engine::~Engine()
{
}
