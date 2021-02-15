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
        /*
        Decode data packed by the struct module of Python
        */

        /**
         * Decode bool value
         *
         * Corresponding Python format: "<B"
         */
        bool Decode_Bool(std::istream& is);
        /**
         * Decode bool value
         *
         * Corresponding Python format: "<I"
         */
        std::uint32_t Decode_U32LE(std::istream& is);
        /* Auxiliary function  */
        std::string Decode_CxxStr(std::istream& is);
        /* Auxiliary function  */
        srtl::string_tree<std::string> Decode_SRStrTree(std::istream& is);
    } // namespace detailed
} // namespace srose::locale


#endif
