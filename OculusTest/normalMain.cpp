#include "normalMain.h"

#include <memory>

#include "NormalCamera.h"
#include "NormalDisplay.h"

#include "Engine.h"


using namespace OVR;

int main() {
	int width = 800;
	int height = 600;

	std::unique_ptr<NormalDisplay> normalDisplay(new NormalDisplay(width, height));
	normalDisplay->init();

	std::unique_ptr<Camera> camera(new NormalCamera(normalDisplay->getRenderingTarget()));;

	Engine engine(*normalDisplay, *camera);
	engine.run();

	return 0;
}
