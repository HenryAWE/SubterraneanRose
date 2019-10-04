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
    void parse_content(util::string_tree<std::string>& context, const std::string& content);
} // namespace srose::locale


#endif
