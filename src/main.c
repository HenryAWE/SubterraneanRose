/**
 * @file main.c
 * @author HenryAWE
 * @brief Program entry
 */

#include <stddef.h>
#include <stdlib.h>
#include <SDL.h>


int main(int argc, char* argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{ // Init failed
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"SDL_Init() failed",
			SDL_GetError(),
			NULL
			);

		exit(EXIT_FAILURE);
	}
	else
	{
		atexit(&SDL_Quit);
	}

	SDL_Window* win = SDL_CreateWindow(
		"Subterranean Rose",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_OPENGL
		);
	if(!win)
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"SDL_CreateWindow() failed",
			SDL_GetError(),
			NULL
			);

		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
