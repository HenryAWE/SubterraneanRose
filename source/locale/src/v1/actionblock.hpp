/**
 * @file actionblock.hpp
 * @author HenryAWE
 * @brief Action block v1
 */

#ifndef SROSE_LOCALE_v1_actionblock_hpp_
#define SROSE_LOCALE_v1_actionblock_hpp_

#include <iosfwd>
#include <string>
#include <optional>
#include <sr/locale/language.hpp>


namespace srose::locale::v1
{
    class ActionBlock
    {
    public:
        ActionBlock(std::istream& is);

        ~ActionBlock();

        TextErrorAction text_error = SRLC_RETURN_EMPTY_STRING;
        std::optional<std::string> error_string;
        std::optional<std::string> fallback;

    private:
        void Decode(std::istream& is);
    };
} // namespace srose::locale::v1


#endif
