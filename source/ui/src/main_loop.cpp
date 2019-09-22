/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief 
 */

#include "main_loop.hpp"
#include <cstdlib>
#include <sr/wm/event.h>
#include <sr/sys/version_info.h>
#include <imgui.h>


int SRSCALL main_loop(SR_WM_display* display)
{
    bool show_about = false;
    bool show_imgui_demo = false;

    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();

        /*Main Menu Bar */
        if(ImGui::BeginMainMenuBar())
        {
            ImGui::TextColored(
                {0.01f, 0.549f, 0.85f, 1},
                "SR - %d.%d.%d",
                SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
            );
            ImGui::Separator();

            if(ImGui::BeginMenu("File##SR"))
            {
                if(ImGui::Checkbox("Show ImGui Demo", &show_imgui_demo)){}
                ImGui::Separator();

                if(ImGui::MenuItem("Quit"))
                    loop = SDL_FALSE;

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Window##SR"))
            {
                if(ImGui::MenuItem("Toggle full screen"))
                {
                    SDL_SetWindowFullscreen(
                        display->win,
                        !(SDL_GetWindowFlags(display->win)&SDL_WINDOW_FULLSCREEN)
                    );
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();

            if(ImGui::BeginMenu("Help##SR"))
            {
                if(ImGui::MenuItem("Home page"))
                {
                    #ifdef __WINDOWS__
                    std::system("explorer https://github.com/HenryAWE/SubterraneanRose");
                    #elif defined __LINUX__
                    std::system("xdg-open https://github.com/HenryAWE/SubterraneanRose");
                    #endif
                }
                ImGui::Separator();
                if(ImGui::MenuItem("About"))
                    show_about = true;
                ImGui::EndMenu();
            }
            ImGui::Separator();
            ImGui::EndMainMenuBar();
        }

        if(show_about&&ImGui::Begin("About Subterranean Rose##SR", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            if(ImGui::IsItemClicked())
                show_about = false;
            ImGui::TextColored(
                {0, 0.749f, 1, 1},
                "Subterranean Rose - %d.%d.%d",
                SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
            );

            ImGui::TextWrapped(
                "2D particle editor and player\n"
                "Home page: https://github.com/HenryAWE/SubterraneanRose\n\n"
                "Git commit ID: %I64x",
                SR_SYS_GitCommitID()
            );
            ImGui::End();
        }

        if(show_imgui_demo)
            ImGui::ShowDemoWindow();
        SR_WM_EndFrame();

        /*Rendering */
        SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
        SDL_RenderClear(display->renderer);
        SR_WM_RenderFrame();
        SDL_RenderPresent(display->renderer);
    }

    return EXIT_SUCCESS;
}
