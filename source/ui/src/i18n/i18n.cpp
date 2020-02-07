/**
 * @file i18n.cpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#include "i18n.hpp"
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <sr/locale/locale.hpp>
#include <boost/locale.hpp>
#include <sr/ui/console/cmdline.hpp>


namespace srose::ui
{
    class LocaleNotFound : public std::runtime_error
    {
    public:
        LocaleNotFound() : runtime_error("Locale not found") {}
    };

    static std::map<std::string, std::shared_ptr<locale::Language>> g_lang_map;
    static std::shared_ptr<locale::Language> g_default_lang;

    locale::Language* GetDefaultLanguage() noexcept
    {
        return g_default_lang.get();
    }

    void LoadAllLanguage(const std::filesystem::path& lcres)
    {
        namespace fs = filesystem;
        if(!fs::exists(lcres))
            throw LocaleNotFound();

        // Iterate through the locale directory
        for(auto dt : fs::directory_iterator(lcres))
        {
            if(fs::is_directory(dt.path()) || dt.path().extension()!=".srlc") continue;
            auto lang = std::make_shared<locale::Language>(dt.path());

            g_lang_map[lang->gettext("srose.language.iso")] = std::move(lang);
        }

        if(g_lang_map.size() == 0) // Nothing was loaded
            throw LocaleNotFound();
    }
    void SelectLanguage()
    {
        // Set a specific locale as default locale
        auto sys_lc = locale::GetSystemLocale();
        if(g_lang_map.count("en"))
            g_default_lang = g_lang_map["en"];
        g_default_lang = GetNearestLanguage(sys_lc);
        if(!g_default_lang)
            throw LocaleNotFound();
        std::locale::global(locale::CreateTranslation(sys_lc, g_default_lang));
    }

    std::shared_ptr<locale::Language> GetNearestLanguage(std::locale lc)
    {
        std::string preferred = console::GetPreferredLanguage();
        auto& lc_info = std::use_facet<boost::locale::info>(lc);
        std::vector<std::pair<std::string /*name*/, int /*weight*/>> lcs;
        lcs.reserve(g_lang_map.size() + 1);
        for(auto& i : g_lang_map)
            lcs.emplace_back(std::make_pair(i.first, 0));
        for(auto& j : lcs)
        { // Calculate locale's weight
            std::string_view sv = j.first;
            if(sv=="C" || sv.size()==0) continue;
            if(!preferred.empty() && preferred==sv) { j.second = 10; continue; }
            std::size_t sep_pos = sv.find_first_of("_-");
            if(sv.substr(0, sep_pos) == lc_info.language())
                ++j.second;
            if(sep_pos==sv.npos || sep_pos==sv.size()) continue;
            ++sep_pos;
            if(sv.substr(sep_pos)==lc_info.country() || sv.substr(sep_pos)==lc_info.variant())
                ++j.second;
        }

        // Chose the most suitable locale based on the weight
        lcs.erase(std::unique(lcs.begin(), lcs.end()), lcs.end());
        if(lcs.size() == 0) return g_default_lang;
        std::sort(lcs.begin(), lcs.end(), [](auto& r, auto& l)->bool { return r.second>l.second; });
        if(lcs[0].second != 0) lcs.erase(std::remove_if(lcs.begin(), lcs.end(), [](auto& i)->bool { return i.second==0; }), lcs.end());
        if(lcs.size() == 0) return g_default_lang;

        return g_lang_map[lcs[0].first];
    }
} // namespace srose::ui
