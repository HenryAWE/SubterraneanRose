/**
 * @file buildinfo.hpp
 * @author HenryAWE
 * @brief Outputting build information to command line
 */

#ifndef SROSE_CONSOLE_buildinfo_hpp_
#define SROSE_CONSOLE_buildinfo_hpp_

#include <string>


namespace srose::console
{
    std::string GenerateCppInfo();
    std::string GenerateLibInfo();
} // namespace srose::console


#endif
