/**
 * @file common.cpp
 * @author HenryAWE
 * @brief Common path
 */

#include <sr/filesystem/common.hpp>


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
    path GetWritableFolder()
    {
        return GetAppData()/"Subterranean Rose";
    }
    path GetLogFolder()
    {
        return GetWritableFolder()/"log";
    }
} // namespace srose::filesystem
