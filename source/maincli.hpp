/**
 * @file maincli.hpp
 * @author HenryAWE
 * @brief Command-line handling
 */

#ifndef SROSE_maincli_hpp_
#define SROSE_maincli_hpp_

#include <sr/progopt/cmdline.hpp>


namespace srose
{
    void HandleConsoleArg(progopt::CommandLineInterface& cli);
} // namespace srose


#endif
