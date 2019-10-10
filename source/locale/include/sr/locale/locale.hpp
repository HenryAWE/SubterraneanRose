/**
 * @file locale.hpp
 * @author HenryAWE
 * @brief Locale support
 */

#ifndef SROSE_LOCALE_locale_hpp_
#define SROSE_LOCALE_locale_hpp_

#include <locale>
#include <sr/core/macros.h>


namespace srose::locale
{
    std::locale SRSCALL get_system_locale(bool utf8 = true);
} // namespace srose::locale


#endif
