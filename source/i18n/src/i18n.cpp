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
#include <sr/trace/log.hpp>


extern std::stringstream GetEmbeddedLang();

namespace srose::i18n
{
    
    class LocaleNotFound : public std::runtime_error
    {
    public:
        LocaleNotFound() : runtime_error("Locale not found") {}
    };

    static std::shared_ptr<locale::Language> CreateBuiltinLang()
    {
        auto ss = GetEmbeddedLang();
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
            try
            {
                auto lang = std::make_shared<locale::Language>(dt.path());

                g_lang_set.insert(std::move(lang));
            }
            catch(const std::exception&)
            {
                BOOST_LOG_TRIVIAL(warning)
                    << "[i18n] Load \"" << dt.path().u8string() << "\" failed";
            }
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
        auto result = locale::SearchClosest(g_lang_set, locale_name);
        if(!result)
            result = g_default_lang;
        return std::move(result);
    }

    const locale::LanguageSet& GetLanguageSet() noexcept
    {
        return g_lang_set;
    }
} // namespace srose::i18n
