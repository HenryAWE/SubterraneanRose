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
    /**
     * @brief Load all language resources
     * 
     * @param lcres The path to the resources
     */
    void LoadAllLanguage(const std::filesystem::path& lcres);

    /**
     * @brief Get the default language
     * 
     * @return locale::language* The pointer to the default language (never null)
     */
    locale::language* GetDefaultLanguage() noexcept;
    /**
     * @brief Get the nearest language based on the specific locale
     * 
     * @param lc The specific locale
     * @return std::shared_ptr<locale::language> The nearest language, or "C" on error
     */
    std::shared_ptr<locale::language> GetNearestLanguage(std::locale lc);
} // namespace srose::ui


#endif
