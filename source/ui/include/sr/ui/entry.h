/**
 * @file entry.h
 * @author HenryAWE
 * @brief Program entry
 */

#ifndef SROSE_UI_entry_h_
#define SROSE_UI_entry_h_

#include <sr/core/macros.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Program entry on desktop
 */
int SRSCALL program_entry(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif
