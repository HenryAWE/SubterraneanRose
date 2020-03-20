/**
 * @file initialize.hpp
 * @author HenryAWE
 * @brief Initializer
 */

#ifndef SROSE_UI_initialize_hpp_
#define SROSE_UI_initialize_hpp_

#include <sr/wm/display.hpp>
#include <sr/filesystem/filesystem.hpp>


namespace srose
{
    void SetWorkingDirectory(const char* argv0);

    void InitializeAllSystems(wm::Display* display);
    void DeinitializeAllSystems() noexcept;
} // namespace srose


#endif
