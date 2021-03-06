#pragma once

#include "SDL_video.h"
#include "Display.h"
#include "NormalRenderingTarget.h"

namespace video {
    class RenderingTarget;

    class NormalDisplay: public Displayy
    {
        SDL_Window* window;
        SDL_GLContext context;
        int width;
        int height;
        NormalRenderingTarget renderingTarget;
    public:
        NormalDisplay(int width, int height);
        void init();
        void swap();
        int getWidth() { return width; }
        int getHeight() { return height; }
        RenderingTarget& getRenderingTarget() {
            return renderingTarget;
        }
        ~NormalDisplay();
    };
}
