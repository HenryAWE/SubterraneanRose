/**
 * @file progopt.hpp
 * @author HenryAWE
 * @brief Program options
 */

#ifndef SROSE_CONSOLE_progopt_hpp_
#define SROSE_CONSOLE_progopt_hpp_

#include <sr/core/macros.hpp>


namespace srose::console
{
    /**
     * @brief Parse console commands
     * 
     * @param argc The count of arguments
     * @param argv The arguments list
     * @return int Zero means to continue, 1 means program should quit
     */
    int SRSCALL ParseArg(int argc,char* argv[]) noexcept;

    /**
     * @brief Show window in fullscreen
     * 
     * @return int 1 means fullscrenn mode is required
     */
    int SRSCALL FullscreenRequired() noexcept;
    /**
     * @brief Is debug UI required
     * 
     * @return int 1 means debug UI is required
     */
    int SRSCALL DrawDebugOverlay() noexcept;
} // namespace srose::console


#endif
