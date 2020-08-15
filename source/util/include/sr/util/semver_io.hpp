/**
 * @file semver_io.hpp
 * @author HenryAWE
 * @brief Input/Output support of semantic version library
 */

#ifndef SROSE_UTIL_HEADERS_semver_io_hpp_
#define SROSE_UTIL_HEADERS_semver_io_hpp_

#include <iostream>
#include "semver.hpp"


namespace srose::util
{
    std::ostream& operator<<(std::ostream& os, const SemVer& semver)
    {
        return os
            << semver.major()
            << '.'
            << semver.minor()
            << '.'
            << semver.patch();
    }
} // namespace srose::util


#endif
