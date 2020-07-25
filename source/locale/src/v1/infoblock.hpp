/**
 * @file infoblock.cpp
 * @author HenryAWE
 * @brief Information block v1
 */

#ifndef SROSE_LOCALE_v1_infoblock_hpp_
#define SROSE_LOCALE_v1_infoblock_hpp_

#include <iosfwd>
#include <string>
#include <sr/util/semver.hpp>


namespace srose::locale::v1
{
    class InfoBlock
    {
    public:
        InfoBlock(std::istream& is);

        ~InfoBlock();

        std::string id;
        std::string name;
        util::SemVer version;

    private:
        void Decode(std::istream& is);
    };
} // namespace srose::locale::v1


#endif
