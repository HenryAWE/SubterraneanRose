/**
 * @file language.cpp
 * @author HenryAWE
 * @brief Language
 */

#include <sr/locale/language.hpp>
#include <sr/locale/parser.hpp>
#include <cstring>
#include <fstream>
#include <boost/locale.hpp>


namespace srose::locale
{
    Language::Language()
    {
        m_tr.emplace_at("srose.language.name", "Default");
        m_tr.emplace_at("srose.language.iso", "C");
    }

    Language::Language(const filesystem::path& file)
    {
        std::ifstream ifs(file, std::ios_base::binary);
        if(!ifs.good())
            throw std::runtime_error("[locale] Load " + file.u8string() + " failed");
        Load(ifs);
    }
    Language::Language(std::istream& is)
    {
        Load(is);
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
    std::string Language::gettext(std::string_view path, use_fallback_t)
    {
        auto opt = m_tr.get_value_optional(path);
        if(opt.has_value())
            return std::move(*opt);

        if(m_fallback)
        {
            opt = m_fallback->m_tr.get_value_optional(path);
            if(opt.has_value())
                return std::move(*opt);
        }

        if(!m_default.has_value())
            throw util::string_tree_base::path_not_found("path not found");
        return *m_default;
    }
    std::string Language::gettext(std::string_view path, std::string_view alternate)
    {
        auto opt = m_tr.get_value_optional(path);
        return opt.has_value() ? *opt : std::string(alternate);
    }

    void Language::Load(std::istream& is)
    {
        is.exceptions(std::ios_base::failbit);
        /* Check the header */
        char header[5]{};
        is.read(header, 4);
        if(strncmp(header, "SRLC", 4) != 0)
            throw std::runtime_error("[locale] Corrupted locale file");
        /* Version number */
        int major = detailed::Decode_U32LE(is);
        int minor = detailed::Decode_U32LE(is);
        int patch = detailed::Decode_U32LE(is);
        assert(major == 0);
        assert(minor == 1);
        assert(patch == 0);

        char subheader[5]{};
        is.read(subheader, 4);
        if(strncmp(subheader, "@txt", 4) == 0)
        {
            try
            {
                while(is.good())
                {
                    m_tr.merge(detailed::Decode_SRStrTree(is));
                }
            }
            catch(std::ios_base::failure&) {}
            LoadSpecStrings();
        }
    }
    void Language::LoadSpecStrings()
    {
        m_name = gettext("srose.language.name", "Default");
        m_iso = gettext("srose.language.iso", "C");
        m_default = m_tr.get_value_optional("srose.language.default");
    }

    std::locale SRSCALL CreateTranslation(const std::locale& in, std::shared_ptr<Language> lang)
    {
        return std::locale(in, new TranslationFacet(std::move(lang)));
    }

    // Initiate static member
    std::locale::id TranslationFacet::id;

    std::string TranslationHelper::str() const
    {
        return std::use_facet<TranslationFacet>(std::locale()).get().gettext(path);
    }
} // namespace srose::locale
