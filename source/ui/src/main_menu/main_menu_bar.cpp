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
    MainMenuBar::MainMenuBar(SR_WM_display* disp)
        : display(disp)
    {
        LoadStrings();
    }

    void MainMenuBar::LoadStrings()
    {
       file = GetDefaultLanguage()->gettext("srose.ui.main-menu.file", "File") + "###MMBFile";
       quit = GetDefaultLanguage()->gettext("srose.ui.main-menu.file.quit", "Quit") + "###MMBQuit";
       window = GetDefaultLanguage()->gettext("srose.ui.main-menu.window", "Window") + "###MMBWindow";
       toggle_screen = GetDefaultLanguage()->gettext("srose.ui.main-menu.window.toggle-screen", "Toggle Fullscreen") + "###MMBToggleScreen";
       dev = GetDefaultLanguage()->gettext("srose.ui.main-menu.dev", "Developer") + "###MMBDev";
       help = GetDefaultLanguage()->gettext("srose.ui.main-menu.help", "Help") + "###MMBHelp";
       home_page = GetDefaultLanguage()->gettext("srose.ui.main-menu.help.home-page", "Home Page") + "###MMBHomePage";
       about_sr = GetDefaultLanguage()->gettext("srose.ui.main-menu.help.about-sr", "About") + "###MMBAbout";
    }

    void MainMenuBar::Update()
    {
        if(!ImGui::BeginMainMenuBar())
            return;
        /*Brief version info */
        ImGui::TextColored(
            {0.01f, 0.549f, 0.85f, 1}, // Deep blue
            "SR - %d.%d.%d",
            SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
        );
        ImGui::Separator();
        /*---------- */

        if(ImGui::BeginMenu(file.c_str()))
        {
            if(ImGui::MenuItem(quit.c_str()))
            {
                SDL_Event quit_event{};
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu(window.c_str()))
        {
            if(ImGui::MenuItem(toggle_screen.c_str()))
            {
                SDL_SetWindowFullscreen(
                    display->win,
                    !(SDL_GetWindowFlags(display->win)&SDL_WINDOW_FULLSCREEN)
                );
            }

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu(dev.c_str()))
        {
            if(ImGui::Checkbox("Dear ImGui Demo", &show_imgui_demo)){}
            if(ImGui::Checkbox("Debug UI", &show_debug_ui)){}

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu(help.c_str()))
        {
            if(ImGui::MenuItem(home_page.c_str()))
            {
                SR_UTIL_OpenInBrowser("https://github.com/HenryAWE/SubterraneanRose");
            }

            if(ImGui::MenuItem(about_sr.c_str()))
                show_about = true;
            ImGui::EndMenu();
        }

        if(show_about)
            ImGui::OpenPopup("About Subterranean Rose");
        AboutPopup();

        ImGui::EndMainMenuBar();
    }

    void MainMenuBar::AboutPopup()
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
                show_about = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
} // namespace srose::ui::main_menu
