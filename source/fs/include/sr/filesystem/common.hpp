/**
 * @file common.hpp
 * @author HenryAWE
 * @brief Common path
 */

#ifndef SROSE_FILESYSTEM_common_hpp_
#define SROSE_FILESYSTEM_common_hpp_

#include <filesystem>


namespace srose::filesystem
{
    using namespace std::filesystem;

    path GetResourceFolder();
    path GetLocaleFolder();
} // namespace srose::filesystem


#endif
