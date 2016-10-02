#pragma once
#include "SDL.h"

#include "Scene.h"

class InputManager {
    private:
        Scene& scene;
        float x;
        float y;
        Uint32 lastTicks;
        bool shouldQuit;
        SDL_GameController *controller;

        void quit();
	public:
        InputManager(Scene& scene);
        void init();
        void processInput();
        bool isShouldQuit() { return shouldQuit; }
        ~InputManager();
};
