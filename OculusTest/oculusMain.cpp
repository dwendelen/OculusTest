#include <memory>

#include "Oculus.h"
#include "OculusCamera.h"
#include "OculusDisplay.h"
#include "NormalDisplay.h"

#include "Engine.h"

#include "normalMain.h"

using namespace OVR;

int WinMain() {

	bool useOculus = true;
	if (useOculus) {
		int width = 800;
		int height = 600;

		std::unique_ptr<NormalDisplay> normalDisplay = std::unique_ptr<NormalDisplay>(new NormalDisplay(width, height));
		normalDisplay->init();

		Oculus oculus = Oculus();
		oculus.init();
		ovr_SetInt(oculus.getSession(), OVR_PERF_HUD_MODE, 0);
		std::unique_ptr<OculusDisplay> display = std::unique_ptr<OculusDisplay>(new OculusDisplay(oculus, normalDisplay.get()));
		display->init();

		std::unique_ptr<Camera> camera = std::unique_ptr<Camera>(new OculusCamera(oculus, display->getRenderingTargets()));;

		Engine engine(&oculus, display.get(), camera.get());
		engine.run();

	}
	else {
		main();
	}
	return 0;
}