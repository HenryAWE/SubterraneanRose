/**
 * @file cmdline.cpp
 * @author HenryAWE
 * @brief Functions for parsed command line arguments
 */

#include <sr/ui/console/cmdline.hpp>
#include <boost/program_options.hpp>


namespace srose::ui::console
{
#if !defined SROSE_DISABLE_CUI // Controlled by the build flag SROSE_DISABLE_CUI
    extern "C"
    const boost::program_options::variables_map& GetVariablesMapInternal() noexcept;

    std::string GetPreferredLanguage()
    {
        if(GetVariablesMapInternal().count("language"))
        {
            std::string lang = GetVariablesMapInternal()["language"].as<std::string>();
            return lang=="auto" ? std::string() : std::move(lang);
        }
        else
        {
            return std::string();
        }
    }
#else
    std::string GetPreferredLanguage()
    {
        return std::string();
    }
#endif
} // namespace srose::ui::console
