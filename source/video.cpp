/**
 * @file video.cpp
 * @author HenryAWE
 * @brief Video options
 */

#include "video.hpp"
#include <SDL.h>
#include <iostream>
#include <fmt/core.h>
#include <sr/locale/language.hpp>


namespace srose
{
    void Video_GetDisplayMode(int disp_index, std::ostream& os)
    {
        auto _ = [&lang = std::use_facet<locale::TranslationFacet>(os.getloc())](std::string_view path)
        {
            return lang.get().GetText(path);
        };

        int dm_count = SDL_GetNumDisplayModes(disp_index);
        if(dm_count < 1)
        {
            os
                << fmt::format(
                    "Get number of the display modes of the display-{:02d} failed: {}",
                    disp_index,
                    SDL_GetError()
                )
                << std::endl;

            return;
        }

        const char* disp_name = SDL_GetDisplayName(disp_index);

        os
            << fmt::format(
                _("srose.cli.video.get-display-mode.display-name"),
                disp_index,
                disp_name ? disp_name : "(failed)"
            )
            << std::endl
            << fmt::format(_("srose.cli.video.get-display-mode.display-mode-count"), dm_count)
            << std::endl
            << _("srose.cli.video.get-display-mode.format-desc")
            << std::endl;

        std::string display_mode_fmt = _("srose.cli.video.get-display-mode.format");
        for(int i = 0; i < dm_count; ++i)
        {
            SDL_DisplayMode mode{};
            if(SDL_GetDisplayMode(disp_index, i, &mode) != 0)
            {
                os
                    << fmt::format("{:03d}:\tfailed - {}\n", i, SDL_GetError())
                    << std::endl;

                continue;
            }

            os
                << fmt::format(
                    display_mode_fmt,
                    i,
                    SDL_BITSPERPIXEL(mode.format),
                    SDL_GetPixelFormatName(mode.format) + sizeof("SDL_PIXELFORMAT"),
                    mode.w,
                    mode.h,
                    mode.refresh_rate
                )
                << std::endl;
        }
    }
} // namespace srose
