/**
 * @file main_menu_bar.cpp
 * @author HenryAWE
 * @brief Main menu bar
 */

#include "main_menu_bar.hpp"
#include <imgui.h>
#include <SDL.h>
#include <sr/core/version_info.h>
#include <sr/util/open_in_browser.h>
#include "../i18n/i18n.hpp"


namespace srose::ui::main_menu
{
    void SRSCALL LoadStrings(MainMenuBarContext* ctx)
    {
        assert(ctx);
        ctx->file = GetDefaultLanguage()->gettext("srose.ui.main-menu.file", "File") + "##SRMMB";
        ctx->quit = GetDefaultLanguage()->gettext("srose.ui.main-menu.file.quit", "Quit");
        ctx->window = GetDefaultLanguage()->gettext("srose.ui.main-menu.window", "Window") + "##SRMMB";
        ctx->toggle_screen = GetDefaultLanguage()->gettext("srose.ui.main-menu.window.toggle-screen", "Toggle Fullscreen");
        ctx->dev = GetDefaultLanguage()->gettext("srose.ui.main-menu.dev", "Developer") + "##SRMMB";
        ctx->help = GetDefaultLanguage()->gettext("srose.ui.main-menu.help", "Help") + "##SRMMB";
        ctx->home_page = GetDefaultLanguage()->gettext("srose.ui.main-menu.help.home-page", "Home page");
        ctx->about_sr = GetDefaultLanguage()->gettext("srose.ui.main-menu.help.about-sr", "About");
    }

    static void SRSCALL AboutPopup(MainMenuBarContext* ctx)
    {
        ImGui::SetNextWindowPosCenter(true);
        if(ImGui::BeginPopupModal("About Subterranean Rose", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::TextColored(
                {0.01f, 0.549f, 0.85f, 1}, // Deep blue
                "Subterranean Rose %d.%d.%d - %s",
                SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH,
                SR_CORE_GitCommitShortID()
            );
            ImGui::Separator();
            /*---------- */

            ImGui::TextWrapped(
                "[WIP] An open-source 2D particle editor and player\n"
            );

            if(ImGui::Button("OK"))
            {
                ctx->show_about = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void SRSCALL MainMenuBar(MainMenuBarContext* ctx, SR_WM_display* display)
    {
        if(ImGui::BeginMainMenuBar())
        {
            /*Brief version info */
            ImGui::TextColored(
                {0.01f, 0.549f, 0.85f, 1}, // Deep blue
                "SR - %d.%d.%d",
                SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
            );
            ImGui::Separator();
            /*---------- */

            if(ImGui::BeginMenu(ctx->file.c_str()))
            {
                if(ImGui::MenuItem(ctx->quit.c_str()))
                {
                    SDL_Event quit_event{};
                    quit_event.type = SDL_QUIT;
                    SDL_PushEvent(&quit_event);
                }

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu(ctx->window.c_str()))
            {
                if(ImGui::MenuItem(ctx->toggle_screen.c_str()))
                {
                    SDL_SetWindowFullscreen(
                        display->win,
                        !(SDL_GetWindowFlags(display->win)&SDL_WINDOW_FULLSCREEN)
                    );
                }

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu(ctx->dev.c_str()))
            {
                if(ImGui::Checkbox("Dear ImGui Demo", &ctx->show_imgui_demo)){}

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu(ctx->help.c_str()))
            {
                if(ImGui::MenuItem(ctx->home_page.c_str()))
                {
                    SR_UTIL_OpenInBrowser("https://github.com/HenryAWE/SubterraneanRose");
                }

                if(ImGui::MenuItem(ctx->about_sr.c_str()))
                    ctx->show_about = true;
                ImGui::EndMenu();
            }

            if(ctx->show_about)
                ImGui::OpenPopup("About Subterranean Rose");
            AboutPopup(ctx);

            ImGui::EndMainMenuBar();
        }
    }
} // namespace srose::ui::main_menu
