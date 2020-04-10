/**
 * @file buildinfo.hpp
 * @author HenryAWE
 * @brief Outputting build information to command line
 */

#ifndef SROSE_UI_CONSOLE_buildinfo_hpp_
#define SROSE_UI_CONSOLE_buildinfo_hpp_

#include <string>


namespace srose::ui::console
{
    std::string GenerateCppInfo();
    std::string GenerateLibInfo();
} // namespace srose::ui::console


#endif
