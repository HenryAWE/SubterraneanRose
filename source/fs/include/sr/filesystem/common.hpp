/**
 * @file common.hpp
 * @author HenryAWE
 * @brief Common path
 */

#ifndef SROSE_FILESYSTEM_common_hpp_
#define SROSE_FILESYSTEM_common_hpp_

#include <filesystem>
#include "platform.hpp"


namespace srose::filesystem
{
    using namespace std::filesystem;

    path GetResourceFolder();
    path GetLocaleFolder();
    path GetWritableFolder();
    path GetLogFolder();
} // namespace srose::filesystem


#endif
