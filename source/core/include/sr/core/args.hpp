/**
 * @file args.hpp
 * @author HenryAWE
 * @brief Managing console argument
 */

#ifndef SROSE_CORE_args_h_
#define SROSE_CORE_args_h_

#include "macros.hpp"


namespace srose::core
{
   int SRSCALL GetArgc() noexcept;
    char** SRSCALL GetArgv() noexcept; 
} // namespace srose::core


#endif
