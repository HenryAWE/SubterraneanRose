/**
 * @file args.c
 * @author HenryAWE
 * @brief Managing console argument
 */

#include <sr/core/args.h>
#include <stddef.h>
#include <assert.h>


// Global variables make it easy to access the console argument
int sr_argc = 0;
char** sr_argv = NULL;

int SRSCALL SR_CORE_GetArgc()
{
    assert(sr_argc != 0);
    return sr_argc;
}
char** SRSCALL SR_CORE_GetArgv()
{
    assert(sr_argv != NULL);
    return sr_argv;
}
