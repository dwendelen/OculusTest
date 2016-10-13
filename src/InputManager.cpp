#include "InputManager.h"
#include "Extras/OVR_Math.h"

using namespace OVR;

namespace input {

    InputManager::InputManager(Scene& scene, VR& vr):
        x(0),
        y(0),
        lastTicks(0),
        scene(scene),
        vr(vr),
        shouldQuit(false),
        controller(NULL),
        wireframe(false)
    {

    }
    void InputManager::maybeAddController(int i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
        }
    }
    void InputManager::init()
    {
        SDL_GameController *controller = NULL;
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            maybeAddController(i);
            if (controller) {
                break;
            }
        }
        if(!controller) {
            y = 1.0f;
            x = 0.8f;
        }
    }

    void InputManager::processInput() {
            Uint32 ticks = SDL_GetTicks();
            Uint32 elapsedMilis = ticks - lastTicks;
            lastTicks = ticks;
            SDL_Event windowEvent;

            while (SDL_PollEvent(&windowEvent))
            {
                switch (windowEvent.type) {
                    case SDL_QUIT:
                        quit();
                        return;
                    case SDL_KEYDOWN:
                        switch(windowEvent.key.keysym.scancode) {
                            case SDL_SCANCODE_ESCAPE:
                                quit();
                                return;
                        }
                        break;
                    case SDL_CONTROLLERDEVICEADDED:
                        if(!controller) {
                            int index = windowEvent.cdevice.which;
                            maybeAddController(index);
                        }
                        break;
                    case SDL_CONTROLLERBUTTONDOWN:
                        switch (windowEvent.cbutton.button) {
                            case SDL_CONTROLLER_BUTTON_Y:
                                    wireframe = !wireframe;
                                    if(wireframe) {
                                        scene.enableWireframe();
                                    } else {
                                        scene.disableWireframe();
                                    }
                                    break;
                            case SDL_CONTROLLER_BUTTON_A:
                                    scene.place();
                                    break;
                            case SDL_CONTROLLER_BUTTON_X:
                                    scene.changeColor();
                                    break;
                            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                                    scene.move(Vector3f(0, 0, -0.016f));
                                    break;
                            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                                    scene.move(Vector3f(0, 0, 0.016f));
                                    break;
                            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                                    scene.move(Vector3f(-0.032f, 0, 0));
                                    break;
                            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                                    scene.move(Vector3f(0.032f, 0, 0));
                                    break;
                        }
                        break;
                    case SDL_CONTROLLERBUTTONUP:
                        switch (windowEvent.cbutton.button) {
                            case SDL_CONTROLLER_BUTTON_BACK:
                                vr.recenter();
                                break;
                            case SDL_CONTROLLER_BUTTON_START:
                                quit();
                                return;
                        }
                        break;
                    case SDL_CONTROLLERAXISMOTION:
                        switch (windowEvent.caxis.axis) {
                        case SDL_CONTROLLER_AXIS_LEFTX:
                            if (abs(windowEvent.caxis.value) < 3000) {
                                x = 0;
                            }
                            else {
                                x = windowEvent.caxis.value / 32767.0f;
                            }
                            break;
                        case SDL_CONTROLLER_AXIS_LEFTY:
                            if (abs(windowEvent.caxis.value) < 3000) {
                                y = 0;
                            }else {

                                y = windowEvent.caxis.value / 32767.0f;
                            }
                                break;
                        }
                        break;
                }
            }

            float radiantsPerMilisec = 0.003f;
            Quatf rotation = Quatf(Vector3f(0, 1, 0), radiantsPerMilisec * elapsedMilis * x);
            rotation *= Quatf(Vector3f(1, 0, 0), radiantsPerMilisec * elapsedMilis * y);
            scene.rotate(rotation);
    }

    void InputManager::quit()
    {
        shouldQuit = true;
    }

    InputManager::~InputManager() {
        SDL_GameControllerClose(controller);
    }
}
