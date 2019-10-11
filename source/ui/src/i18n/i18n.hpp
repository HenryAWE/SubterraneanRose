/**
 * @file i18n.hpp
 * @author HenryAWE
 * @brief i18n support for GUI
 */

#ifndef SROSE_UI_I18N_i18n_hpp_
#define SROSE_UI_I18N_i18n_hpp_

#include <filesystem>


namespace srose::ui
{
    void LoadAllLanguage(const std::filesystem::path& lcres);
} // namespace srose::ui


#endif
