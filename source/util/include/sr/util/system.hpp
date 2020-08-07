/**
 * @file system.hpp
 * @author HenryAW
 * @brief System
 */

#ifndef SROSE_UTIL_system_hpp_
#define SROSE_UTIL_system_hpp_

#include <string>
#include <optional>


namespace srose::util
{
    /**
     * @brief Get environmental variable
     * 
     * @param name Name of the variable
     * @return std::optional<std::string> The data of the variable encoded in UTF-8,
     *                                    or std::nullopt if the variable doesn't exist
     */
    std::optional<std::string> GetEnv(const std::string& name);
} // namespace srose::util


#endif
