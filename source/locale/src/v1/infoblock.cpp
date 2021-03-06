/**
 * @file infoblock.cpp
 * @author HenryAWE
 * @brief Information block v1
 */

#include "infoblock.hpp"
#include <iostream>
#include <sr/locale/parser.hpp>


namespace srose::locale::v1
{
    InfoBlock::InfoBlock(std::istream& is)
    {
        Decode(is);
    }

    InfoBlock::~InfoBlock() = default;

    void InfoBlock::Decode(std::istream& is)
    {
        try
        {
            id = detailed::Decode_CxxStr(is);
            is_complete = detailed::Decode_Bool(is);
            name = detailed::Decode_CxxStr(is);
            version.major() = detailed::Decode_U32LE(is);
            version.minor() = detailed::Decode_U32LE(is);
            version.patch() = detailed::Decode_U32LE(is);
            author = detailed::Decode_CxxStr(is);
            comment = detailed::Decode_CxxStr(is);
        }
        catch(std::ios_base::failure&) {}
    }
} // namespace srose::locale::v1
