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
    Language::Language() = default;
    Language::Language(const Language& other)
        : m_id(other.m_id),
        m_name(other.m_name),
        m_text(other.m_text),
        m_fallback(other.m_fallback) {}
    Language::Language(Language&& move) noexcept
        : m_id(std::move(move.m_id)),
        m_name(std::move(move.m_name)),
        m_text(std::move(move.m_text)),
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

    std::string Language::GetText(std::string_view path)
    {
        return GetTextWith(path, m_text_error);
    }
    std::string Language::GetTextWith(std::string_view path, TextErrorAction action)
    {
        auto opt = GetTextOptional(path);
        if(opt)
            return std::move(*opt);

        switch(action)
        {
        default:
        case SRLC_RETURN_EMPTY_STRING:
            return std::string();

        case SRLC_RETURN_REQUEST:
            return std::string(path);

        case SRLC_RETURN_ERROR_STRING:
            if(m_error_string)
                return *m_error_string;
            else
                throw std::invalid_argument("[locale] m_error_string == std::nullopt");

        case SRLC_USE_FALLBACK:
            if(m_fallback)
                return m_fallback->GetText(path);
            else
                throw std::invalid_argument("[locale] m_fallback == nullptr");

        case SRLC_THROW_EXCEPTION:
            throw std::out_of_range("[locale] Path \"" + std::string(path) + "\" not found");
        }
    }
    std::string Language::GetTextOr(std::string_view path, std::string_view alternative)
    {
        auto opt = GetTextOptional(path);
        if(opt)
            return std::move(*opt);
        else
            return std::string(alternative);
    }
    std::optional<std::string> Language::GetTextOptional(std::string_view path)
    {
        return m_text.get_value_optional(path);
    }

    void Language::LinkFallback(std::shared_ptr<Language> lang)
    {
        if(lang.get() == this)
            return;
        if(lang == nullptr)
        {
            m_fallback.reset();
            return;
        }

        // Check for circular dependency
        std::vector<Language*> history;
        auto current = lang.get();
        while(current)
        {
            history.push_back(current);
            if(current->GetTextErrorAction() != SRLC_USE_FALLBACK)
                break;
            else if(current == this)
                throw CircularDependency("[locale] Circular dependency", std::move(history));
            current = current->m_fallback.get();
        }

        m_fallback.swap(lang);
    }
    void Language::LinkFallback(LanguageSet& langs)
    {
        if(!m_fallback_id)
            return;
        auto result = SearchClosest(langs, *m_fallback_id);
        if(result)
           LinkFallback(std::move(result));
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
                m_id = std::move(inf.id);
                m_name = std::move(inf.name);
                m_version = inf.version;
                m_author = std::move(inf.author);
                m_comment = std::move(inf.comment);
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
    }
    void Language::DecodeTextBlock(std::istream& is)
    {
        m_text.merge(detailed::Decode_SRStrTree(is));
    }

    CircularDependency::CircularDependency(const std::string& message, std::vector<Language*> history_)
        : logic_error(message), history(history_) {}

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
        return std::use_facet<TranslationFacet>(std::locale()).get().GetText(path);
    }
} // namespace srose::locale
