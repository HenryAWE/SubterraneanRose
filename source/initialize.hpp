/**
 * @file initialize.hpp
 * @author HenryAWE
 * @brief Initializer
 */

#ifndef SROSE_initialize_hpp_
#define SROSE_initialize_hpp_

#include <sr/wm/window.hpp>
#include <sr/filesystem/filesystem.hpp>


namespace srose
{
    void SetWorkingDirectory(const char* argv0);

    void InitializeAllSystems(wm::Window& window, bool disable_audio);
    void DeinitializeAllSystems(wm::Window& window, bool disable_audio) noexcept;
} // namespace srose


#endif
