#pragma once
#include "SDL.h"

#include "Scene.h"
#include "VR.h"

class InputManager {
    private:
        Scene& scene;
        VR& vr;

        float x;
        float y;
        Uint32 lastTicks;
        bool shouldQuit;
        SDL_GameController *controller;
        bool wireframe;

        void quit();
	public:
        InputManager(Scene& scene, VR& vr);
        void init();
        void processInput();
        bool isShouldQuit() { return shouldQuit; }
        ~InputManager();
};
