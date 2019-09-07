/**
 * @file main.c
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/player/entry.h>


/*Program entry */
int SDL_main(int argc, char* argv[])
{
    //TODO: Handling other platform
    return entry_windows(argc, argv);
}
