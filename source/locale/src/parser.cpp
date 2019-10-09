/**
 * @file parser.cpp
 * @author HenryAWE
 * @brief Translation file parser
 */

#define BOOST_SPIRIT_UNICODE 1
#include <sr/locale/parser.hpp>
#ifdef _MSC_VER
#   pragma warning(disable:4828) // Disable code page warning
#endif
#include <cwctype>
#include <sstream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_standard_wide.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix.hpp>
#include <boost/locale/encoding_utf.hpp>


namespace srose::locale
{
    struct translation
    {
        std::wstring identifier, text;
    };
} // namespace srose::locale

BOOST_FUSION_ADAPT_STRUCT(
    srose::locale::translation,
    (std::wstring, identifier)
    (std::wstring, text)
);

namespace srose::locale
{
    using namespace boost::spirit;

    template <typename Iterator>
    class translation_parser : public qi::grammar<Iterator, translation(), standard_wide::space_type>
    {
        qi::rule<Iterator, std::wstring(), standard_wide::space_type> identifier;
        qi::rule<Iterator, std::wstring(), standard_wide::space_type> quoted_strings;
        qi::rule<Iterator, translation(), standard_wide::space_type> start;
    public:
        translation_parser() : translation_parser::base_type(start)
        {
            identifier %= qi::lexeme[+(standard_wide::char_ - '"' - '=' -'@' -' ')];
            quoted_strings %= qi::lexeme[+('"' >> +(standard_wide::char_ - '"') >> '"')];

            start %= '@' >> identifier >> '=' >> quoted_strings;
        }
    };

    void parse_string(util::string_tree<std::string>& context, const std::wstring& content)
    {
        using boost::spirit::standard_wide::space;
        typedef std::wstring::const_iterator iterator_type;
        typedef translation_parser<iterator_type> parser_type;
        parser_type grammer;

        translation tr;
        bool r = qi::phrase_parse(content.cbegin(), content.cend(), grammer, space, tr);
        if(!r)
        { // Failed
            return;
        }

        context.emplace_at(
            boost::locale::conv::utf_to_utf<char>(tr.identifier),
            boost::locale::conv::utf_to_utf<char>(tr.text)
        );
    }

    bool is_comment(const std::wstring& str)
    {
        for(wchar_t c : str)
        {
            if(std::iswspace(c)) continue;
            else if(c == L'#') return true; // Comment

            return false;
        }
        return true; // Empty string, also treat it as comment
    }

    util::string_tree<std::string> parse_stream(std::istream& is)
    {
        util::string_tree<std::string> result;
        std::wstring wstr;
        while(!is.eof())
        {
            std::string str;
            std::getline(is, str);

            wstr = boost::locale::conv::utf_to_utf<wchar_t>(str);
            if(is_comment(wstr)) continue;

            parse_string(result, wstr);
        }

        return std::move(result);
    }
    util::string_tree<std::string> parse_wstream(std::wistream& is)
    {
        util::string_tree<std::string> result;
        std::wstring wstr;
        while(!is.eof())
        {
            std::getline(is, wstr);
            if(is_comment(wstr)) continue;

            parse_string(result, wstr);
        }

        return std::move(result);
    }
} // namespace srose::locale
