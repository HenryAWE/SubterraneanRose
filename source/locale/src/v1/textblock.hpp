/**
 * @file textblock.hpp
 * @author HenryAWE
 * @brief Text block v1
 */

#ifndef SROSE_LOCALE_V1_testblock_hpp_
#define SROSE_LOCALE_V1_testblock_hpp_

#include <iosfwd>
#include <srtl/string_tree.hpp>


namespace srose::locale::v1
{
    class TextBlock
    {
    public:
        TextBlock(std::istream& is);

        ~TextBlock();

        srtl::string_tree<std::string> texts;

    private:
        void Decode(std::istream& is);
    };
} // namespace srose::locale::v1

#endif
