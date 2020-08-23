/**
 * @file maincli.cpp
 * @author HenryAWE
 * @brief Command-line handling
 */

#include "maincli.hpp"
#include <SDL.h>
#include <fmt/core.h>
#include <sr/i18n/i18n.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/shell.hpp>
#include "video.hpp"


namespace srose
{
    void HandleConsoleArg(progopt::CommandLineInterface& cli)
    {
        if(cli.GetBool("lang-available").value_or(false))
        {
            cli.WinRequestConsole(true);

            auto& os = cli.GetOutputStream();
            const auto& lang_set = i18n::GetLanguageSet();
            const auto builtin = i18n::GetBuiltinLanguage();
            const std::string lcfmt = "{}\t: {}";

            os << SRTR("srose.cli.lang.available.built-in") << std::endl;
            os << fmt::format(lcfmt, builtin->GetId(), builtin->GetName()) << std::endl;

            os << SRTR("srose.cli.lang.available.installed") << std::endl;
            for(auto& i : lang_set)
            {
                if(i == builtin)
                    continue;
                os << fmt::format(lcfmt, i->GetId(), i->GetName()) << std::endl;
            }

            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.Exists("video-get-display-mode"))
        {
            auto disp_index = cli.GetInt("video-get-display-mode");
            if(!disp_index)
                return;

            cli.WinRequestConsole(true);
            if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
            {
                cli.GetOutputStream()
                    << "Initialize SDL VIDEO subsystem failed: "
                    << SDL_GetError()
                    << std::endl;
            }
            else
            {
                try
                {
                    Video_GetDisplayMode(std::max(*disp_index, 0), cli.GetOutputStream());
                }
                catch(...)
                {
                    SDL_QuitSubSystem(SDL_INIT_VIDEO);
                    throw;
                }
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
            }
            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.GetBool("print-appdata").value_or(false))
        {
            cli.WinRequestConsole(true);
            cli.GetOutputStream() << filesystem::GetAppData().u8string() << std::endl;
            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.GetBool("explore-appdata").value_or(false))
        {
            util::OpenInBrowser(filesystem::GetAppData().u8string().c_str());
            cli.RequestQuit();
        }
    }
} // namespace srose
