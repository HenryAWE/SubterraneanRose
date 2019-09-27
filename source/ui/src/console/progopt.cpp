/**
 * @file progopt.cpp
 * @author HenryAWE
 * @brief Program options
 */

#include <sr/ui/console/progopt.h>
#include <boost/program_options.hpp>
#include <sstream>
#include <cstdio>
#include <sr/core/version_info.h>
#ifndef SR_ENABLE_CUI
#   error Build CUI module without defining requested macro
#endif


int SRSCALL SR_UI_CONSOLE_ParseArg(int argc, char* argv[])
{
    try
    {
        namespace po = boost::program_options;

        po::options_description desc("Information");
        desc.add_options()
            ("help", "This message")
            ("version", "Version information");
        
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help"))
        {
            std::stringstream ss;
            ss << desc;
            std::printf(ss.str().c_str());

            return 1;
        }
        if(vm.count("version"))
        {
            std::printf(
                "%d.%d.%d - %s\n",
                SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH,
                SR_CORE_GitCommitShortID()
            );

            return 1;
        }

        return 0;
    }
    catch(...)
    {
        return 0;
    }
}
