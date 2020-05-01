/**
 * @file entry.hpp
 * @author HenryAWE
 * @brief Program entry
 */

#ifndef SROSE_UI_entry_hpp_
#define SROSE_UI_entry_hpp_

#include <sr/core/macros.hpp>
#include <sr/wm/window.hpp>


namespace srose
{
    /**
     * @brief Program entry
     */
    int SRSCALL ProgramEntry(wm::Window& window) noexcept;
} // namespace srose::ui


#endif
