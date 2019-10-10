/**
 * @file locale.cpp
 * @author HenryAWE
 * @brief Locale support
 */

#include <sr/locale/locale.hpp>
#include <boost/locale.hpp>


namespace srose::locale
{
    std::locale SRSCALL get_system_locale(bool utf8)
    {
        using namespace boost::locale;
        generator gen;
        return gen(boost::locale::util::get_system_locale(utf8));
    }
} // namespace srose::locale
