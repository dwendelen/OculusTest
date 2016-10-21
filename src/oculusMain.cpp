#include <memory>

#include "Oculus.h"
#include "OculusCamera.h"
#include "OculusDisplay.h"
#include "NormalDisplay.h"

#include "Engine.h"
#include "MemoryManager.h"
#include "LegoBrick.h"
#include "NormalRenderer.h"

using namespace std;
using namespace memory;
using namespace OVR;
using namespace oculus;
using namespace video;

int main(int argc, char* argv[]) {
    int width = 800;
    int height = 600;

    std::unique_ptr<NormalDisplay> normalDisplay(new NormalDisplay(width, height));
    normalDisplay->init();

    Oculus oculus = Oculus();
    oculus.init();
    ovr_SetInt(oculus.getSession(), OVR_PERF_HUD_MODE, 0);
    std::unique_ptr<OculusDisplay> display(new OculusDisplay(oculus, normalDisplay.get()));
    display->init();

    std::unique_ptr<Camera> camera (new OculusCamera(oculus, display->getRenderingTargets()));;

	std::unique_ptr<LegoBrick> brick(new LegoBrick());
	brick->init();
	std::unique_ptr<MemoryManager> memory(new MemoryManager());
	memory->init();
	memory->load(*brick);

	std::unique_ptr<NormalRenderer> renderer(new NormalRenderer(*memory, *brick));
	renderer->init();

    Engine engine(*display, *camera, oculus, *renderer, *brick);
    engine.run();
	return 0;
}
