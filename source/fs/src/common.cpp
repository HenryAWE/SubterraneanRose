/**
 * @file common.cpp
 * @author HenryAWE
 * @brief Common path
 */

#include <sr/filesystem/common.hpp>
#include <sr/filesystem/platform.hpp>


namespace srose::filesystem
{
    path GetResourceFolder()
    {
        return current_path()/"resource";
    }
    path GetLocaleFolder()
    {
        return current_path()/"locale";
    }
    path GetAppData()
    {
        return GetSysAppData()/"Subterranean Rose";
    }
    path GetLogFolder()
    {
        return GetAppData()/"log";
    }
} // namespace srose::filesystem
