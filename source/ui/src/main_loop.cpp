/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief 
 */

#include <glad/glad.h>
#include "main_loop.hpp"
#include <cstdlib>
#include <sr/wm/event.h>
#include <imgui.h>
#include "main_menu/main_menu_bar.hpp"
#include <sr/gpu/opengl3/texture.hpp>
#include <sr/locale/locale.hpp>
#include "i18n/i18n.hpp"


int SRSCALL main_loop(SR_WM_display* display)
{
    using namespace srose::ui;

    main_menu::MainMenuBarContext main_menu_ctx{};

    LoadAllLanguage("locale");

    glClearColor(0, 0, 0, 1);
    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();

        main_menu::MainMenuBar(&main_menu_ctx, display);

        if(main_menu_ctx.show_imgui_demo)
            ImGui::ShowDemoWindow();
        SR_WM_EndFrame();

        /*Rendering */
        glClear(GL_COLOR_BUFFER_BIT);
        SR_WM_RenderFrame();
        SDL_GL_SwapWindow(display->win);
    }

    return EXIT_SUCCESS;
}
