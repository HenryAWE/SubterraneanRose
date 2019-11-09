/**
 * @file language.cpp
 * @author HenryAWE
 * @brief Language
 */

#include <sr/locale/language.hpp>
#include <sr/locale/parser.hpp>
#include <fstream>
#include <sstream>


namespace srose::locale
{
    Language::Language()
    {
        m_tr.emplace_at("srose.language.name", "Default");
        m_tr.emplace_at("srose.language.iso", "C");
    }

    Language::Language(const std::filesystem::path& directory)
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

        LoadSpecStrings();
    }

    std::string Language::gettext(std::string_view path)
    {
        if(m_default.has_value())
        {
            auto opt = m_tr.get_value_optional(path);
            assert(m_default.has_value());
            return opt.has_value()? *opt : *m_default;
        }
        else
        {
            return m_tr.get_value(path);
        }
    }
    std::string Language::gettext(std::string_view path, std::string_view alternate)
    {
        auto opt = m_tr.get_value_optional(path);
        return opt.has_value() ? *opt : std::string(alternate);
    }

    void Language::LoadSpecStrings()
    {
        m_name = gettext("srose.language.name", "Default");
        m_iso = gettext("srose.language.iso", "C");
        m_default = m_tr.get_value_optional("srose.language.default");
    }

    // Initiate static member
    std::locale::id TranslationFacet::id;

    std::string TranslationHelper::str() const
    {
        return std::use_facet<TranslationFacet>(std::locale()).get().gettext(path);
    }
} // namespace srose::locale
