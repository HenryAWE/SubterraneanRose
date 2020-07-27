/**
 * @file language.cpp
 * @author HenryAWE
 * @brief Language
 */

#include <sr/locale/language.hpp>
#include <sr/locale/parser.hpp>
#include <cstring>
#include <fstream>
#include <regex>
#include <boost/locale.hpp>
#include "v1/infoblock.hpp"
#include "v1/actionblock.hpp"
#include "v1/textblock.hpp"


#define SROSE_LOCALE_BACKEND_API_VERSION 1

namespace srose::locale
{
    Language::Language()
    {
        m_text.emplace_at("srose.language.name", "Default");
        m_text.emplace_at("srose.language.iso", "C");
    }
    Language::Language(const Language& other)
        : m_id(other.m_id),
        m_name(other.m_name),
        m_text(other.m_text),
        m_default(other.m_default),
        m_fallback(other.m_fallback) {}
    Language::Language(Language&& move) noexcept
        : m_id(std::move(move.m_id)),
        m_name(std::move(move.m_name)),
        m_text(std::move(move.m_text)),
        m_default(std::move(move.m_default)),
        m_fallback(std::move(move.m_fallback)) {}
    Language::Language(const filesystem::path& file)
    {
        std::ifstream ifs(file, std::ios_base::binary);
        if(!ifs.good())
            throw std::runtime_error("[locale] Load " + file.u8string() + " failed");
        Decode(ifs);
    }
    Language::Language(std::istream& is)
    {
        Decode(is);
    }

    std::string Language::gettext(std::string_view path)
    {
        if(m_default.has_value())
        {
            auto opt = m_text.get_value_optional(path);
            assert(m_default.has_value());
            return opt.has_value()? *opt : *m_default;
        }
        else
        {
            return m_text.get_value(path);
        }
    }
    std::string Language::gettext(std::string_view path, use_fallback_t)
    {
        auto opt = m_text.get_value_optional(path);
        if(opt.has_value())
            return std::move(*opt);

        if(m_fallback)
        {
            opt = m_fallback->m_text.get_value_optional(path);
            if(opt.has_value())
                return std::move(*opt);
        }

        if(!m_default.has_value())
            throw util::string_tree_base::path_not_found("path not found");
        return *m_default;
    }
    std::string Language::gettext(std::string_view path, std::string_view alternate)
    {
        auto opt = m_text.get_value_optional(path);
        return opt.has_value() ? *opt : std::string(alternate);
    }

    void Language::LinkFallback(LanguageSet& langs)
    {
        if(!m_fallback_id)
            return;
        auto result = SearchClosest(langs, *m_fallback_id);
        if(result)
            m_fallback.swap(result);
    }

    void Language::Decode(std::istream& is)
    {
        /* Check the header */
        char header[5]{};
        is.read(header, 4);
        if(strncmp(header, "SRLC", 4) != 0)
            throw std::runtime_error("[locale] Corrupted locale file");
        /* Version number */
        int backend_version = detailed::Decode_U32LE(is);
        if(backend_version != SROSE_LOCALE_BACKEND_API_VERSION)
            throw std::runtime_error("[locale] Invalid version number");

        while(is.good())
        {
            using namespace v1;
            char subheader[5]{};
            is.read(subheader, 4);
            if(!is.good())
                break;

            if(strncmp(subheader, "@inf", 4) == 0)
            {
                InfoBlock inf(is);
                m_id = inf.id;
                m_name = inf.name;
                m_version = inf.version;
            }
            if(strncmp(subheader, "@act", 4) == 0)
            {
                ActionBlock act(is);
                m_text_error = act.text_error;
                m_error_string = act.error_string;
                m_fallback_id = act.fallback;
            }
            if(strncmp(subheader, "@txt", 4) == 0)
            {
                TextBlock txt(is);
                m_text.merge(std::move(txt.texts));
            }
        }

        LoadSpecStrings();
    }
    void Language::DecodeTextBlock(std::istream& is)
    {
        m_text.merge(detailed::Decode_SRStrTree(is));
    }

    void Language::LoadSpecStrings()
    {
        m_default = m_text.get_value_optional("srose.language.default");
    }

    std::shared_ptr<Language> SearchClosest(LanguageSet& langs, const std::string& id)
    {
        using namespace std;

        const regex id_pattern = regex(R"(^([a-z]*)(?:[_-]([A-Z]*))?.*)");
        if(!regex_match(id, id_pattern)) // Format error
            return nullptr;
        const string id_language = regex_replace(id, id_pattern, "$1");
        const string id_country = regex_replace(id, id_pattern, "$2");

        vector<pair<LanguageSet::iterator /*lang*/, int /*weight*/>> weights;
        weights.reserve(langs.size());
        for(auto i = langs.begin(); i != langs.end(); ++i)
            weights.push_back(pair(i, 0));
        for(auto& [lang, weight] : weights)
        { // Calculate weights
            if(!*lang) // nullptr
                continue;

            const string lang_id = (*lang)->GetId();

            const string lang_language = regex_replace(lang_id, id_pattern, "$1");
            if(lang_language == id_language)
                ++weight;

            if(id_country.empty())
                continue;
            const string lang_country = regex_replace(lang_id, id_pattern, "$2");
            if(lang_country.empty())
                continue;
            if(lang_country == id_country)
                ++weight;
        }

        // Chose the most suitable locale based on the weight
        weights.erase(
            std::unique(weights.begin(), weights.end()),
            weights.end()
        );
        if(weights.size() == 0)
            return nullptr;
        std::sort(
            weights.begin(), weights.end(),
            [](auto& r, auto& l)->bool { return r.second > l.second; }
        );
        weights.erase(
            std::remove_if(
                weights.begin(), weights.end(),
                [](auto& i)->bool { return i.second == 0; }
            ),
            weights.end()
        ); 
        if(weights.size() == 0)
            return nullptr;
        return std::move(*weights[0].first);
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
