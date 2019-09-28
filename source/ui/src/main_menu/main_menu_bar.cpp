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


namespace srose::ui::main_menu
{
    static void SRSCALL AboutPopup()
    {
        if(ImGui::BeginPopupModal("About Subterranean Rose", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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

            if(ImGui::BeginMenu("File##SRMMB"))
            {
                if(ImGui::MenuItem("Quit"))
                {
                    SDL_Event quit_event{};
                    quit_event.type = SDL_QUIT;
                    SDL_PushEvent(&quit_event);
                }

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Window##SRMMB"))
            {
                if(ImGui::MenuItem("Toggle fullscreen"))
                {
                    SDL_SetWindowFullscreen(
                        display->win,
                        !(SDL_GetWindowFlags(display->win)&SDL_WINDOW_FULLSCREEN)
                    );
                }

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Help##SRMMB"))
            {
                if(ImGui::MenuItem("Home page"))
                {
                    SR_UTIL_OpenInBrowser("https://github.com/HenryAWE/SubterraneanRose");
                }
                if(ImGui::Checkbox("Show ImGui demo", &ctx->show_imgui_demo)){}
                ImGui::EndMenu();
            }

           if(ImGui::MenuItem("About"))
               ImGui::OpenPopup("About Subterranean Rose");
           AboutPopup();

            ImGui::EndMainMenuBar();
        }
    }
} // namespace srose::ui::main_menu
