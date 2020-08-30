/**
 * @file parser.cpp
 * @author HenryAWE
 * @brief Translation data parser
 */

#include <sr/locale/parser.hpp>
#include <cstddef>
#include <boost/endian/conversion.hpp>


namespace srose::locale
{
    namespace detailed
    {
        std::uint32_t Decode_U32LE(std::istream& is)
        {
            std::uint32_t ret;
            is.read((char*)&ret, sizeof(ret));
            return boost::endian::little_to_native(ret);
        }
        std::string Decode_CxxStr(std::istream& is)
        {
            const std::uint32_t len = Decode_U32LE(is);
            if(len == 0)
                return std::string();
            std::string ret;
            ret.resize(len);
            is.read(ret.data(), len);
            return std::move(ret);
        }
        srtl::string_tree<std::string> Decode_SRStrTree(std::istream& is)
        {
            srtl::string_tree<std::string> ret;

            std::string subid = Decode_CxxStr(is);
            std::string data = Decode_CxxStr(is);
            if(!data.empty())
                ret.emplace_at(subid, std::move(data));
            auto& subtree = ret.child(subid);
            std::uint32_t children_count = Decode_U32LE(is);
            for(std::uint32_t i = 0; i < children_count; ++i)
            {
                subtree.merge(Decode_SRStrTree(is));
            }

            return std::move(ret);
        }
    } // namespace detailed
} // namespace srose::locale
