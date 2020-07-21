/**
 * @file i18n.hpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#ifndef SROSE_I18N_i18n_hpp_
#define SROSE_I18N_i18n_hpp_

#include <sr/filesystem/filesystem.hpp>
#include <sr/locale/language.hpp>


namespace srose::i18n
{
    /**
     * @brief Load all language resources from the locale resource folder
     * 
     * @param lcres The path to the folder, typically "./locale/"
     */
    void LoadAllLanguage(const std::filesystem::path& lcres);
    /**
     * @brief Select a language as default one based on system setting and command line argument
     * 
     * @param preferred User preferred language, or nullptr for ignoring this argument
     */
    void SelectLanguage(const char* preferred = nullptr);
    /**
     * @brief Set a specific language as default language
     * 
     * @param lang The pointer to the language object, shouldn't be null
     */
    void SelectLanguage(std::shared_ptr<locale::Language> lang);

    /**
     * @brief Get the default language
     * 
     * @return std::shared_ptr<locale::Language> The pointer to the default language (never null)
     * @remark Pointer will be available until program quit
     */
    std::shared_ptr<locale::Language> GetDefaultLanguage() noexcept;
    /**
     * @brief Get the built-in language
     * 
     * @return std::shared_ptr<locale::Language> The pointer to the built-in language (never null)
     * @remark Pointer will be available until program quit
     */
    std::shared_ptr<locale::Language> GetBuiltinLanguage() noexcept;
    /**
     * @brief Get the nearest language based on the specific locale name
     * @remark Only recognize language_COUNTRY format, like zh_CN or en_US
     * 
     * @param locale_name The name of the locale
     * @return std::shared_ptr<locale::Language> The nearest language, or the default language on error
     */
    std::shared_ptr<locale::Language> GetNearestLanguage(std::string locale_name);

    const locale::LanguageSet& GetLanguageSet() noexcept;
} // namespace srose::i18n


#endif
