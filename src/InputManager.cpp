#include "InputManager.h"
#include "Extras/OVR_Math.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace OVR;

namespace input {

    InputManager::InputManager(Scene& scene, VR& vr):
        x(0),
        y(0),
		z(0),
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
        //if(!controller) {
            //y = 1.0f;
            //x = 0.8f;
        //}
    }
	float convertinput(float raw) {
		if (abs(raw) < 3000.0f) {
			return 0;
		}
		else {
			return raw / 32767.0f;
		}
	}
    void InputManager::processInput() {
            Uint32 ticks = SDL_GetTicks();
            Uint32 elapsedMilis = ticks - lastTicks;
            lastTicks = ticks;
            SDL_Event windowEvent;
			if (elapsedMilis != 0) {
				int fps = 1000 / elapsedMilis;
				//cout << "FPS " << fps << endl; 
			}
        
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
							case SDL_SCANCODE_SPACE:
								scene.place();
								break;
                            case SDL_SCANCODE_BACKSPACE:
                                wireframe = !wireframe;
                                if(wireframe) {
                                    scene.enableWireframe();
                                } else {
                                    scene.disableWireframe();
                                }
                                break;
							case SDL_SCANCODE_A:
                                scene.moveUnit(-1, 0, 0);
								break;
							case SDL_SCANCODE_D:
								scene.moveUnit( 1, 0, 0);
								break;
							case SDL_SCANCODE_W:
								scene.moveUnit(0, 0, -1);
								break;
							case SDL_SCANCODE_S:
								scene.moveUnit(0, 0,  1);
								break;
							case SDL_SCANCODE_TAB:
								scene.changeColor();
								break;
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
                                scene.moveUnit(0, 0, -1);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                                scene.moveUnit(0, 0,  1);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                                scene.moveUnit(-1, 0, 0);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                                scene.moveUnit( 1, 0, 0);
                                break;
							case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
								scene.rotate(Quatf(Vector3f(0, 1, 0), -M_PI_2));
								break;
							case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
								scene.rotate(Quatf(Vector3f(0, 1, 0), M_PI_2));
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
							x = convertinput(windowEvent.caxis.value);
                            break;
                        case SDL_CONTROLLER_AXIS_LEFTY:
							z = convertinput(windowEvent.caxis.value);
                            break;
						case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
							y = -convertinput(windowEvent.caxis.value);
							break;
						case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
							y = convertinput(windowEvent.caxis.value);
							break;
                        }
                        break;
                }
            }

            float radiantsPerMilisec = 0.003f;
            float meterPerMilisec = 0.001f;
            //Quatf rotation = Quatf(Vector3f(0, 1, 0), radiantsPerMilisec * elapsedMilis * x);
            //rotation *= Quatf(Vector3f(1, 0, 0), radiantsPerMilisec * elapsedMilis * y);
            //scene.rotate(rotation);
            scene.move(x * meterPerMilisec , y * meterPerMilisec, z * meterPerMilisec);
    }

    void InputManager::quit()
    {
        shouldQuit = true;
    }

    InputManager::~InputManager() {
        SDL_GameControllerClose(controller);
    }
}
