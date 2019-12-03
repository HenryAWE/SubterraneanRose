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
#include <sr/locale/locale.hpp>
#include <boost/locale.hpp>
#include <sr/ui/console/cmdline.hpp>


namespace srose::ui
{
    std::shared_ptr<locale::Language> GetDefaultLanguageInternal();
    std::shared_ptr<locale::Language> GetNearestLanguageInternal(std::locale lc);

    static std::map<std::string, std::shared_ptr<locale::Language>> g_lang_map;
    static std::shared_ptr<locale::Language> g_default_lang;

    void LoadAllLanguage(const std::filesystem::path& lcres)
    {
        namespace fs = std::filesystem;
        if(!fs::exists(lcres))
            goto end;

        for(auto dt : fs::directory_iterator(lcres))
        {
            if(!fs::is_directory(dt.path())) continue;
            auto lang = std::make_shared<locale::Language>(dt.path());

            g_lang_map[lang->gettext("srose.language.iso")] = std::move(lang);
        }

    end:
        if(g_lang_map.size() == 0)
        {
            g_default_lang = g_lang_map["C"] = std::make_shared<locale::Language>();

            return;
        }

        auto sys_lc = locale::GetSystemLocale();
        g_default_lang = GetNearestLanguageInternal(sys_lc);
        std::locale::global(locale::CreateTranslation(sys_lc, g_default_lang));
    }

    std::shared_ptr<locale::Language> GetDefaultLanguageInternal()
    {
        assert(g_default_lang);
        return g_default_lang;
    }

    std::shared_ptr<locale::Language> GetNearestLanguageInternal(std::locale lc)
    {
        std::string preferred = console::GetPreferredLanguage();
        auto& lc_info = std::use_facet<boost::locale::info>(lc);
        std::vector<std::pair<std::string, int>> lcs;
        lcs.reserve(g_lang_map.size() + 1);
        for(auto& i : g_lang_map)
            lcs.emplace_back(std::make_pair(i.first, 0));
        for(auto& j : lcs)
        {
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

        lcs.erase(std::unique(lcs.begin(), lcs.end()), lcs.end());
        if(lcs.size() == 0) return g_lang_map["C"];
        std::sort(lcs.begin(), lcs.end(), [](auto& r, auto& l)->bool { return r.second>l.second; });
        if(lcs[0].second != 0) lcs.erase(std::remove_if(lcs.begin(), lcs.end(), [](auto& i)->bool { return i.second==0; }), lcs.end());
        if(lcs.size() == 0) return g_lang_map["C"];

        return g_lang_map[lcs[0].first];
    }

    locale::Language* GetDefaultLanguage() noexcept
    {
        return g_default_lang.get();
    }
    std::shared_ptr<locale::Language> GetNearestLanguage(std::locale lc)
    {
        return std::move(GetNearestLanguageInternal(lc));
    }
} // namespace srose::ui
