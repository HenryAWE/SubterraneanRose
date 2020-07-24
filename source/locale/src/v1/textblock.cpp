/**
 * @file textblock.cpp
 * @author HenryAWE
 * @brief Text block v1
 */

#include "textblock.hpp"
#include <iostream>
#include <sr/locale/parser.hpp>
#include <sr/trace/log.hpp>


namespace srose::locale::v1
{
    TextBlock::TextBlock(std::istream& is)
        : texts()
    {
        Decode(is);
    }

    TextBlock::~TextBlock() = default;

    void TextBlock::Decode(std::istream& is)
    {
        try
        {
            texts = detailed::Decode_SRStrTree(is);
        }
        catch(const std::ios_base::failure& e)
        {
            BOOST_LOG_TRIVIAL(warning)
                << e.code() << " : " << e.what();
        }
    }
} // namespace srose::locale::v1
