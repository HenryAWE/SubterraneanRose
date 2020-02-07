/**
 * @file parser.hpp
 * @author HenryAWE
 * @brief Translation data parser
 */

#ifndef SROSE_LOCALE_parser_hpp_
#define SROSE_LOCALE_parser_hpp_

#include <cstddef>
#include <iostream>
#include <sr/util/string_tree.hpp>


namespace srose::locale
{
    namespace detail
    {
        std::uint32_t Decode_U32LE(std::istream& is);
        std::string Decode_CxxStr(std::istream& is);
        util::string_tree<std::string> Decode_SRStrTree(std::istream& is);
    } // namespace detail

    util::string_tree<std::string> parse_stream(std::istream& is);
    util::string_tree<std::string> ParseTranslation(std::istream& is);
} // namespace srose::locale


#endif
