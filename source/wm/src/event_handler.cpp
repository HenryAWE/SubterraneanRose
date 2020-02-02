/**
 * @file event_handler.cpp
 * @author HenryAWE
 * @brief Event handler
 */

#include <imgui.h>
#include <SDL.h>
#include <cassert>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <sr/wm/event.h>
#include <SDL_events.h>


static ImGuiContext* g_imctx = nullptr;
static SR_WM_display* g_display = nullptr;

int SRSCALL SR_WM_InitEventSystem(SR_WM_display* display)
{
    assert(!g_imctx);
    assert(display->win);
    g_display = display;
    g_imctx = ImGui::CreateContext();
    if(!g_imctx)
    {
        return -1;
    }

    if(!ImGui_ImplSDL2_InitForOpenGL(display->win, display->glctx))
    {
        return -1;
    }
    if(!ImGui_ImplOpenGL3_Init(nullptr))
    {
        return -1;
    }

    return 0;
}

SR_WM_display* SRSCALL SR_WM_GetDisplay()
{
    return g_display;
}

void SRSCALL SR_WM_QuitEventSystem()
{
    assert(g_imctx);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(g_imctx);
    g_imctx = nullptr;
    g_display = nullptr;
}

int SRSCALL SR_WM_ProcessEvent()
{
    bool retval = SDL_TRUE;
    SDL_Event event{};
    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch(event.type)
        {
        case SDL_QUIT:
            retval = SDL_FALSE;
            break;
        }
    }

    return (int)retval;
}

void SRSCALL SR_WM_NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(g_display->win);
    ImGui::NewFrame();
}

void SRSCALL SR_WM_EndFrame()
{
    ImGui::EndFrame();
}

void SRSCALL SR_WM_RenderFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
