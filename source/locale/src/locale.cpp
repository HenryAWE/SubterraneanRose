/**
 * @file locale.cpp
 * @author HenryAWE
 * @brief Locale support
 */

#include <sr/locale/locale.hpp>
#include <boost/locale.hpp>


namespace srose::locale
{
    namespace blc = boost::locale;

    std::string SRSCALL GetSystemLocaleName(bool utf8)
    {
        return blc::util::get_system_locale(utf8);
    }

    std::locale SRSCALL GetSystemLocale(bool utf8)
    {
        std::string name = GetSystemLocaleName(utf8);

        return GenerateLocale(name);
    }

    std::locale SRSCALL GenerateLocale(const std::string& id)
    {
        blc::generator gen;

        std::locale lc = gen(id);
        return blc::util::create_info(lc, id);;
    }
} // namespace srose::locale
