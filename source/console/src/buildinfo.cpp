/**
 * @file buildinfo.cpp
 * @author HenryAWE
 * @brief Outputting build information to command line
 */

#ifndef SROSE_DISABLE_CLI

#include <string>
#include <sstream>
#include <boost/version.hpp>
#include <boost/config.hpp>
#include <imgui.h>


namespace srose::console
{
    std::string GenerateCppInfo()
    {
        std::stringstream ss;
        ss
            << "__cplusplus: " << __cplusplus << std::endl
            << "Compiler: " << BOOST_COMPILER << std::endl
            << "Standard library: " << BOOST_STDLIB << std::endl;

        return ss.str();
    }
    std::string GenerateLibInfo()
    {
        std::stringstream ss;
        ss
            << "Boost version: "
            << (BOOST_VERSION / 100000) << '.'
            << ((BOOST_VERSION / 100) % 1000) << '.'
            << (BOOST_VERSION % 100) << ' '
            << '(' << BOOST_VERSION << ')'
            << std::endl;

        ss
            << "ImGui version: "
            << IMGUI_VERSION << ' '
            << '(' << IMGUI_VERSION_NUM << ')'
            << std::endl;

        return ss.str();
    }
} // namespace srose::console

#endif
