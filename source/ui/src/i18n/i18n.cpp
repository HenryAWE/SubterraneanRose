/**
 * @file i18n.cpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#include "i18n.hpp"
#include <map>
#include <memory>
#include <sr/locale/language.hpp>


namespace srose::ui
{
    static std::map<std::string, std::shared_ptr<locale::language>> g_lang_map;

    void LoadAllLanguage(const std::filesystem::path& lcres)
    {
        namespace fs = std::filesystem;
        fs::directory_iterator iter(lcres);

        for(auto dt : fs::directory_iterator(lcres))
        {
            if(!fs::is_directory(dt.path())) continue;
            auto lang = std::make_shared<locale::language>(dt.path());

            g_lang_map[lang->gettext("srose.language.iso")] = std::move(lang);
        }
    }
} // namespace srose::ui
