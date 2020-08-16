/**
 * @file parser.hpp
 * @author HenryAWE
 * @brief Translation data parser
 */

#ifndef SROSE_LOCALE_parser_hpp_
#define SROSE_LOCALE_parser_hpp_

#include <cstddef>
#include <iostream>
#include <srtl/string_tree.hpp>


namespace srose::locale
{
    namespace detailed
    {
        std::uint32_t Decode_U32LE(std::istream& is);
        std::string Decode_CxxStr(std::istream& is);
        srtl::string_tree<std::string> Decode_SRStrTree(std::istream& is);
    } // namespace detailed

    srtl::string_tree<std::string> ParseTranslation(std::istream& is);
} // namespace srose::locale


#endif
