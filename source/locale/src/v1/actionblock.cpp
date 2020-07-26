/**
 * @file actionblock.cpp
 * @author HenryAWE
 * @brief Action block v1
 */

#include "actionblock.hpp"
#include <iostream>
#include <sr/locale/parser.hpp>


namespace srose::locale::v1
{
    ActionBlock::ActionBlock(std::istream& is)
    {
        Decode(is);
    }

    ActionBlock::~ActionBlock() = default;

    void ActionBlock::Decode(std::istream& is)
    {
        text_error = (TextErrorAction)detailed::Decode_U32LE(is);
        if(text_error == SRLC_RETURN_ERROR_STRING)
            error_string.emplace(detailed::Decode_CxxStr(is));
        else if(text_error == SRLC_USE_FALLBACK)
            fallback.emplace(detailed::Decode_CxxStr(is));
    }
} // namespace srose::locale::v1
