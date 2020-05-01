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
    static wm::Window* g_window = nullptr;

    int SRSCALL InitEventSystem(wm::Window& window)
    {
        assert(!g_imctx);
        assert(window.handle());
        g_window = &window;
        g_imctx = ImGui::CreateContext();
        if(!g_imctx)
        {
            return -1;
        }

        if(!ImGui_ImplSDL2_InitForOpenGL(window.handle(), window.glctx()))
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
        g_window = nullptr;
    }

    void SRSCALL NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_window->handle());
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
