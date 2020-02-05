/**
 * @file i18n.hpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#ifndef SROSE_UI_I18N_i18n_hpp_
#define SROSE_UI_I18N_i18n_hpp_

#include <sr/filesystem/filesystem.hpp>
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
     * @return locale::Language* The pointer to the default language (never null)
     */
    locale::Language* GetDefaultLanguage() noexcept;
    /**
     * @brief Get the nearest language based on the specific locale
     * 
     * @param lc The specific locale
     * @return std::shared_ptr<locale::Language> The nearest language, or "C" on error
     */
    std::shared_ptr<locale::Language> GetNearestLanguage(std::locale lc);
} // namespace srose::ui


#endif
