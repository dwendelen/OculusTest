#include "NormalDisplay.h"
#include "SDL.h"
#include "GL/glew.h"

#include <string>
#include <stdexcept>

using namespace std;

namespace video
{
    NormalDisplay::NormalDisplay(int width, int height): width(width), height(height), renderingTarget(NormalRenderingTarget(width, height))
    {
    }

    void NormalDisplay::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
            throw new runtime_error("Could not init SDL: " + string(SDL_GetError()));
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window = SDL_CreateWindow("OpenGL", 100, 100, width, height, SDL_WINDOW_OPENGL);
        if (window == NULL) {
            throw new runtime_error("Could not create window:" + string(SDL_GetError()));
        }

        context = SDL_GL_CreateContext(window);
        if (context == NULL) {
            throw new runtime_error("Could not create OpenGL context: " + string(SDL_GetError()));
        }

        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            throw new runtime_error("Error initializing GLEW: " + string((char *)glewGetErrorString(glewError)));
        }

        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR) {
            cerr << "Opengl error caused by Glew: " << err << endl;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        glFrontFace(GL_CCW);

        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    void NormalDisplay::swap()
    {
        static bool swap = false;
        swap = !swap;
        if (swap) {
            SDL_GL_SwapWindow(window);
        }
    }

    NormalDisplay::~NormalDisplay()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}
