/**
 * @file open_in_browser.hpp
 * @author HenryAWE
 * @brief Open URL in system browser
 */

#ifndef SROSE_UTIL_open_in_browser_hpp_
#define SROSE_UTIL_open_in_browser_hpp_

#include <sr/core/macros.hpp>


namespace srose::util
{
    void SRSCALL OpenInBrowser(const char* url) noexcept;

} // namespace srose::util


#endif
