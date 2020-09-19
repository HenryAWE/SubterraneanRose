/**
 * @file init.hpp
 * @author HenryAWE
 * @brief Initializaton
 */

#ifndef SROSE_CORE_init_hpp_
#define SROSE_CORE_init_hpp_

#include <sr/core/macros.hpp>
#include <SDL.h>


namespace srose::core
{
    /**
     * @brief Initialize SDL and other SDL library
     * 
     * @param msgbox_on_err Show message box on error
     * @return int Zero for success, other value means an error occurred
     */
    int SRSCALL InitSDL(bool msgbox_on_err) noexcept;
    void SRSCALL InitSDLMixer();
    void SRSCALL QuitSDLMixer() noexcept;

    /**
     * @brief Deinitialize SDL and other SDL library
     */
    void SRSCALL QuitSDL() noexcept;

    /**
     * @brief Initialize OpenGL render context
     * 
     * @warning MUST BE CALLED FROM A THREAD WITH GL CONTEXT!!!
     * 
     * @return int Zero for success, other value means an error occurred
     */
    int SRSCALL InitGL() noexcept;

    /**
     * @brief Initialize everything
     * 
     * @param msgbox_on_err Show message box on error
     * @return int Zero for success, other value means an error occurred
     */
    int SRSCALL Init(int argc, char* argv[], int msgbox_on_error);

    /**
     * @brief Deinitialize everything
     */
    void SRSCALL Quit();
} // namespace srose::core


#endif
