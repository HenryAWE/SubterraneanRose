/**
 * @file open_in_browser.c
 * @author HenryAWE
 * @brief Open URL in system browser
 */

#include <sr/util/open_in_browser.h>
#include <SDL_platform.h>
#include <SDL_syswm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#ifdef __WINDOWS__
#   include <wchar.h>
#   include <shellapi.h>
#endif


void SRSCALL SR_UTIL_OpenInBrowser(const char* url)
{
#ifdef __WINDOWS__
    /*Convert URL to wide char to avoid encoding issue */
    int cbMultiByte = (int)strlen(url);
    int cchWideChar = MultiByteToWideChar(CP_UTF8, 0 , url, cbMultiByte, NULL, 0);
    WCHAR* wurl = malloc((cchWideChar)*sizeof(WCHAR));
    wmemset(wurl, 0, cchWideChar);
    if(MultiByteToWideChar(CP_UTF8, 0, url, cbMultiByte, wurl, cchWideChar) <= 0)
    { // Error
        return;
    }

    ShellExecuteW(
        NULL,
        NULL,
        L"explorer.exe",
        wurl,
        L".",
        SW_NORMAL
    );

    free(wurl);

#elif defined __LINUX__
    size_t buflen = strlen(url) + 10;
    char* buf = malloc()
    snprintf(buf, buflen, "xdg-open %s");
    system(buf);

    free(buf);
#endif
}
