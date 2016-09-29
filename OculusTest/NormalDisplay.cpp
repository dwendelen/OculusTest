#include "NormalDisplay.h"
#include "GL\glew.h"
#include "SDL\SDL.h"

#include <stdexcept>


using namespace std;
NormalDisplay::NormalDisplay(int width, int height): width(width), height(height), renderingTarget(NormalRenderingTarget(width, height))
{
}

void NormalDisplay::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		throw new runtime_error("Could not init SDL: " + string(SDL_GetError()));
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
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

	SDL_GameController *controller = NULL;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			if (controller) {
				break;
			}
		}
	}

	
	int a;
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &a);
	
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		throw new runtime_error("Error initializing GLEW: " + string((char *)glewGetErrorString(glewError)));
	}
	//glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_NEVER);
	//glDepthFunc(GL_GREATER);
	
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


