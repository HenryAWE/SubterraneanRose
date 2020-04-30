/**
 * @file event.cpp
 * @author HenryAWE
 * @brief Event handler
 */

#include <imgui.h>
#include <SDL.h>
#include <cassert>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <sr/wm/event.hpp>
#include <SDL_events.h>


namespace srose::wm
{
    static ImGuiContext* g_imctx = nullptr;
    static Display* g_display = nullptr;

    int SRSCALL InitEventSystem(Display* display)
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

    void SRSCALL QuitEventSystem()
    {
        assert(g_imctx);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(g_imctx);
        g_imctx = nullptr;
        g_display = nullptr;
    }

    void SRSCALL NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_display->win);
        ImGui::NewFrame();
    }

    void SRSCALL EndFrame()
    {
        ImGui::EndFrame();
    }

    void SRSCALL RenderFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
} // namespace srose::wm
