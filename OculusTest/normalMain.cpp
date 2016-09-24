#include "normalMain.h"

#include <memory>

#include "NormalCamera.h"
#include "NormalDisplay.h"

#include "Engine.h"


using namespace OVR;

int main() {
	int width = 800;
	int height = 600;

	std::unique_ptr<NormalDisplay> normalDisplay = std::unique_ptr<NormalDisplay>(new NormalDisplay(width, height));
	normalDisplay->init();

	std::unique_ptr<Camera> camera = std::unique_ptr<Camera>(new NormalCamera(normalDisplay->getRenderingTarget()));;

	Engine engine(nullptr, normalDisplay.get(), camera.get());
	engine.run();

	return 0;
}