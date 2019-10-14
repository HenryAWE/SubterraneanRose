/**
 * @file main.c
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.h>
#include <SDL_main.h>
#include <SDL_syswm.h>


/*Program entry */
int main(int argc, char* argv[])
{
#ifdef __WINDOWS__
    BOOL attached = AttachConsole(ATTACH_PARENT_PROCESS);
    if(attached)
    {
        FILE* dummy = NULL;
        freopen_s(&dummy, "conin$", "r+t", stdin);
        freopen_s(&dummy, "conout$", "w+t", stdout);
        freopen_s(&dummy, "conout$", "w+t", stderr);
        printf("\n");
        if(SetConsoleCP(CP_UTF8) == FALSE)
            printf("[exec] SetConsoleCP(CP_UTF8) failed, some output may not be displayed correctly\n");
    }
#endif
    int exit_code = program_entry(argc, argv);

#ifdef __WINDOWS__
    if(attached)
    {
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
        FreeConsole();
    }
#endif

    return exit_code;
}
