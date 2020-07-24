/**
 * @file i18n.cpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#include <sr/i18n/i18n.hpp>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <sr/locale/locale.hpp>
#include <boost/locale.hpp>


extern std::stringstream GetEmbeddedEnglishLang();

namespace srose::i18n
{
    
    class LocaleNotFound : public std::runtime_error
    {
    public:
        LocaleNotFound() : runtime_error("Locale not found") {}
    };

    static std::shared_ptr<locale::Language> CreateBuiltinLang()
    {
        auto ss = GetEmbeddedEnglishLang();
        return std::make_shared<locale::Language>(ss);
    }

    static std::shared_ptr<locale::Language>& GetBuiltinLang()
    {
        static auto builtin_lang = CreateBuiltinLang();
        return builtin_lang;
    }

    static locale::LanguageSet g_lang_set;
    static std::shared_ptr<locale::Language> g_default_lang;

    std::shared_ptr<locale::Language> GetDefaultLanguage() noexcept
    {
        return g_default_lang ? g_default_lang : GetBuiltinLang();
    }
    std::shared_ptr<locale::Language> GetBuiltinLanguage() noexcept
    {
        return GetBuiltinLang();
    }

    void LoadAllLanguage(const std::filesystem::path& lcres)
    {
        g_lang_set.insert(GetBuiltinLang());

        namespace fs = filesystem;
        if(!fs::exists(lcres))
            return;

        // Iterate through the locale directory
        for(auto dt : fs::directory_iterator(lcres))
        {
            if(fs::is_directory(dt.path()) || dt.path().extension()!=".srlc") continue;
            auto lang = std::make_shared<locale::Language>(dt.path());

            g_lang_set.insert(std::move(lang));
        }
    }
    void SelectLanguage(const char* preferred)
    {
        // Set a specific locale as default locale
        auto sys_lc = locale::GetSystemLocale();
        g_default_lang = nullptr;
        if(preferred)
        { // User preferred language
            g_default_lang = GetNearestLanguage(preferred);
        }
        if(!g_default_lang)
        { // Operating system's setting
            auto& lc_info = std::use_facet<boost::locale::info>(sys_lc);
            g_default_lang = GetNearestLanguage(lc_info.name());
        }
        if(!g_default_lang)
            g_default_lang = GetBuiltinLang();
        std::locale::global(locale::CreateTranslation(sys_lc, g_default_lang));
    }
    void SelectLanguage(std::shared_ptr<locale::Language> lang)
    {
        assert(lang);
        g_default_lang.swap(lang);
        auto sys_lc = locale::GetSystemLocale();
        std::locale::global(locale::CreateTranslation(sys_lc, g_default_lang));
    }

    std::shared_ptr<locale::Language> GetNearestLanguage(std::string locale_name)
    {
        using namespace std;
        regex locale_pattern = regex(R"(^([a-z]*)(?:[_-]([A-Z]*))?.*)");
        if(!regex_match(locale_name, locale_pattern)) // Format error
            return g_default_lang;
        string language_name = regex_replace(locale_name, locale_pattern, "$1");
        string country_name = regex_replace(locale_name, locale_pattern, "$2");

        std::vector<std::pair<std::string /*name*/, int /*weight*/>> lcs_weight;
        lcs_weight.reserve(g_lang_set.size());
        std::for_each(
            g_lang_set.begin(), g_lang_set.end(),
            [&lcs_weight](auto& in){ lcs_weight.push_back(pair(in->GetId(), 0)); }
        );
        for(auto& [name, weight] : lcs_weight)
        { // Calculate locale's weight
            std::string_view view = name;
            if(view=="C" || view.size()==0) continue;
            std::size_t sep_pos = view.find_first_of("_-");
            if(view.substr(0, sep_pos) == language_name)
                ++weight;
            if(sep_pos==view.npos || sep_pos==view.size()) continue;
            ++sep_pos;
            if(view.substr(sep_pos)==country_name)
                ++weight;
        }

        // Chose the most suitable locale based on the weight
        lcs_weight.erase(
            std::unique(lcs_weight.begin(), lcs_weight.end()),
            lcs_weight.end()
        );
        if(lcs_weight.size() == 0) return g_default_lang;
        std::sort(
            lcs_weight.begin(), lcs_weight.end(),
            [](auto& r, auto& l)->bool { return r.second>l.second; }
        );
        lcs_weight.erase(
            std::remove_if(
                lcs_weight.begin(), lcs_weight.end(),
                [](auto& i)->bool { return i.second==0; }
            ),
            lcs_weight.end()
        ); 
        if(lcs_weight.size() == 0) return g_default_lang;
        return *g_lang_set.find(lcs_weight[0].first);
    }

    const locale::LanguageSet& GetLanguageSet() noexcept
    {
        return g_lang_set;
    }
} // namespace srose::i18n
