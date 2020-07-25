/**
 * @file platform.hpp
 * @author HenryAWE
 * @brief Platform specific folders
 */

#include <filesystem>


namespace srose::filesystem
{
    using namespace std::filesystem;

    path GetAppData();
} // namespace srose::filesystem
