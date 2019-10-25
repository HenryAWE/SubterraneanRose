/**
 * @file i18n.hpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#ifndef SROSE_UI_I18N_i18n_hpp_
#define SROSE_UI_I18N_i18n_hpp_

#include <filesystem>
#include <sr/locale/language.hpp>


namespace srose::ui
{
    void LoadAllLanguage(const std::filesystem::path& lcres);

    locale::language* GetDefaultLanguage() noexcept;
    std::shared_ptr<locale::language> GetNearestLanguage(std::locale lc);
} // namespace srose::ui


#endif
