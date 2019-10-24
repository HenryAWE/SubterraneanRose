/**
 * @file fontfile.cpp
 * @author HenryAWE
 * @brief Locating font file
 */

#ifndef SROSE_FILESYSTEM_fontfile_hpp_
#define SROSE_FILESYSTEM_fontfile_hpp_

#include <filesystem>
#include <optional>


namespace srose::filesystem
{
    using namespace std::filesystem;

    std::optional<path> GetFont(const std::string& name);
} // namespace srose::filesystem


#endif
