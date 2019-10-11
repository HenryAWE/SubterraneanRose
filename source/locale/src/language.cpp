/**
 * @file language.cpp
 * @author HenryAWE
 * @brief Language
 */

#include <sr/locale/language.hpp>
#include <sr/locale/parser.hpp>
#include <fstream>


namespace srose::locale
{
    language::language(const std::filesystem::path& directory)
    {
        namespace fs = std::filesystem;
        fs::recursive_directory_iterator search(directory, fs::directory_options::skip_permission_denied);
        for(auto& pt : search)
        {
            if(fs::is_regular_file(pt) && pt.path().extension()==".txt")
            {
                std::ifstream ifs(pt.path(), std::ios_base::binary);
                if(ifs.good())
                    m_tr.merge(parse_stream(ifs));
            }
        }
    }

    std::string language::gettext(std::string_view path)
    {
        return m_tr.get_value(path);
    }
    std::string language::gettext(std::string_view path, std::string_view alternate)
    {
        auto opt = m_tr.get_value_optional(path);
        return opt.has_value() ? *opt : std::string(alternate);
    }
} // namespace srose::locale
