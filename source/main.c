/**
 * @file main.c
 * @author HenryAWE
 * @brief Program entry
 */

#include <stddef.h>
#include <stdlib.h>
#include <SDL.h>
#include <sr/wm/display.h>


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

	SR_WM_display* dis = SR_WM_CreateDisplay("Subterranean Rose", 0);

	int loop = SDL_TRUE;
	SDL_Event event;
	SDL_zero(event);

	while(loop)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				loop = SDL_FALSE;
			}
		}

		SDL_SetRenderDrawColor(dis->renderer, 255, 255, 255, 255);
		SDL_RenderClear(dis->renderer);

		SDL_RenderPresent(dis->renderer);
	}

	SR_WM_DestroyDisplay(dis);

	return EXIT_SUCCESS;
}
