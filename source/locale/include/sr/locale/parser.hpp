/**
 * @file parser.hpp
 * @author HenryAWE
 * @brief Translation file parser
 */

#ifndef SROSE_LOCALE_parser_hpp_
#define SROSE_LOCALE_parser_hpp_

#include <iostream>
#include <sr/util/string_tree.hpp>


namespace srose::locale
{
    void parse_string(util::string_tree<std::string>& context, const std::wstring& content);

    util::string_tree<std::string> parse_stream(std::istream& is);
    util::string_tree<std::string> parse_wstream(std::wistream& is);
} // namespace srose::locale


#endif
