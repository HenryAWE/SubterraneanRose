/**
 * @file args.cpp
 * @author HenryAWE
 * @brief Managing console argument
 */

#include <sr/core/args.hpp>
#include <cstddef>
#include <cassert>


namespace srose::core
{
    // Global variables make it easy to access the console argument
    int sr_argc = 0;
    char** sr_argv = nullptr;

    int SRSCALL GetArgc() noexcept
    {
        assert(sr_argc != 0);
        return sr_argc;
    }
    char** SRSCALL GetArgv() noexcept
    {
        assert(sr_argv != nullptr);
        return sr_argv;
    }
} // namespace srose::core
