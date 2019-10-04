/**
 * @file parser.cpp
 * @author HenryAWE
 * @brief Translation file parser
 */

#include <sr/locale/parser.hpp>
#ifdef _MSC_VER
#   pragma warning(disable:4828) // Disable code page warning
#endif
#include <sstream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix.hpp>


namespace srose::locale
{
    struct translation
    {
        std::string identifier, text;
    };
} // namespace srose::locale

BOOST_FUSION_ADAPT_STRUCT(
    srose::locale::translation,
    (std::string, identifier)
    (std::string, text)
);

namespace srose::locale
{
    using namespace boost::spirit;

    template <typename Iterator>
    class translation_parser : public qi::grammar<Iterator, translation(), ascii::space_type>
    {
        qi::rule<Iterator, std::string(), ascii::space_type> identifier;
        qi::rule<Iterator, std::string(), ascii::space_type> quoted_strings;
        qi::rule<Iterator, translation(), ascii::space_type> start;
    public:
        translation_parser() : translation_parser::base_type(start)
        {
            identifier %= qi::lexeme[+(ascii::char_ - '"' - '=' -'@' -' ')];
            quoted_strings %= qi::lexeme[+('"' >> +(ascii::char_ - '"') >> '"')];

            start %= '@' >> identifier >> '=' >> quoted_strings;
        }
    };

    void parse_content(util::string_tree<std::string>& context, const std::string& content)
    {
        using boost::spirit::ascii::space;
        typedef std::string::const_iterator iterator_type;
        typedef translation_parser<iterator_type> parser_type;
        parser_type grammer;

        translation tr;
        bool r = qi::phrase_parse(content.cbegin(), content.cend(), grammer, space, tr);
        if(!r)
        { // Failed
            return;
        }

        context.emplace_at(tr.identifier, std::move(tr.text));
    }
} // namespace srose::locale
