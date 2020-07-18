/**
 * @file shell.hpp
 * @author HenryAWE
 * @brief Shell API abstraction
 */

#ifndef SROSE_UTIL_shell_hpp_
#define SROSE_UTIL_shell_hpp_

#include <sr/core/macros.hpp>


namespace srose::util
{
    void SRSCALL OpenInBrowser(const char* url) noexcept;

} // namespace srose::util


#endif
