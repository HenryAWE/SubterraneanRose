/**
 * @file locale.hpp
 * @author HenryAWE
 * @brief Locale support
 */

#ifndef SROSE_LOCALE_locale_hpp_
#define SROSE_LOCALE_locale_hpp_

#include <locale>
#include <sr/core/macros.h>
#include "language.hpp"


namespace srose::locale
{
    [[nodiscard]]
    std::string SRSCALL GetSystemLocaleName(bool utf8 = true);
    [[nodiscard]]
    std::locale SRSCALL GetSystemLocale(bool utf8 = true);

    [[nodiscard]]
    std::locale SRSCALL GenerateLocale(const std::string& id);
} // namespace srose::locale


#endif
