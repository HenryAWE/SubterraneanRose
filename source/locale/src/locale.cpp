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

        std::string name = util::get_system_locale(utf8);

        std::locale lc = gen.generate(name);
        return util::create_info(lc, name);
    }
} // namespace srose::locale
