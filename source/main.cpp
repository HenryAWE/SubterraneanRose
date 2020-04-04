/**
 * @file main.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.hpp>
#include <SDL_main.h>
#include <SDL_syswm.h>


/*Program entry */
#ifdef _MSC_VER
#   pragma comment(linker, "/subsystem:windows")
#endif
int main(int argc, char* argv[])
{
    int exit_code = srose::ProgramEntry(argc, argv);

    return exit_code;
}
