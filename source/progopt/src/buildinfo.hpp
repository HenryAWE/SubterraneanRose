/**
 * @file buildinfo.hpp
 * @author HenryAWE
 * @brief Outputting build information to command line
 */

#ifndef SROSE_PROGOPT_buildinfo_hpp_
#define SROSE_PROGOPT_buildinfo_hpp_

#include <string>


namespace srose::progopt
{
    std::string GenerateCppInfo();
    std::string GenerateLibInfo();
} // namespace srose::progopt


#endif
