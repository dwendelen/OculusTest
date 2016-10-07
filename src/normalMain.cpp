#include <memory>

#include "NormalCamera.h"
#include "NormalDisplay.h"

#include "Engine.h"
#include "NullVR.h"

using namespace OVR;

int main(int argc, char* argv[]) {
	int width = 800;
	int height = 600;

	std::unique_ptr<NormalDisplay> normalDisplay(new NormalDisplay(width, height));
	normalDisplay->init();

	std::unique_ptr<Camera> camera(new NormalCamera(normalDisplay->getRenderingTarget()));;
    NullVR nullVR;
	Engine engine(*normalDisplay, *camera, nullVR);
	engine.run();

	return 0;
}
