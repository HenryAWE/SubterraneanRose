/**
 * @file progopt.h
 * @author HenryAWE
 * @brief Program options
 */

#ifndef SROSE_UI_CONSOLE_progopt_h_
#define SROSE_UI_CONSOLE_progopt_h_

#include <sr/sys/macros.h>


#ifndef __cplusplus
extern "C"{
#endif

/**
 * @brief 
 * 
 * @param argc The count of arguments
 * @param argv The arguments list
 * @return int Zero means to continue, 1 means program should quit
 */
int SRSCALL SR_UI_CONSOLE_ParseArg(int argc,char* argv[]);

#ifndef __cplusplus
}
#endif

#endif
