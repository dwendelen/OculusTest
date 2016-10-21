#include <memory>

#include "DefaultCamera.h"
#include "NormalDisplay.h"

#include "Engine.h"
#include "NullVR.h"

#include "MemoryManager.h"
#include "LegoBrick.h"
#include "NormalRenderer.h"

using namespace std;
using namespace OVR;
using namespace video;
using namespace memory;

int main(int argc, char* argv[]) {
	int width = 800;
	int height = 600;

	std::unique_ptr<NormalDisplay> normalDisplay(new NormalDisplay(width, height));
	normalDisplay->init();

	std::unique_ptr<Camera> camera(new DefaultCamera(normalDisplay->getRenderingTarget()));;

	std::unique_ptr<LegoBrick> brick(new LegoBrick());
	brick->init();
	std::unique_ptr<MemoryManager> memory(new MemoryManager());
	memory->init();
	memory->load(*brick);

	std::unique_ptr<NormalRenderer> renderer(new NormalRenderer(*memory, *brick));
	renderer->init();

    NullVR nullVR;
	Engine engine(*normalDisplay, *camera, nullVR, *renderer, *brick);
	engine.run();

	return 0;
}
