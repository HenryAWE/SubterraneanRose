/**
 * @file renderer.cpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#include <sr/gpu/opengl3/renderer.hpp>
#include <glad/glad.h>
#include <SDL.h>
#include <sr/ui/gui/uimgr.hpp>
#include "gl_assert.h"
#include "demo.hpp"


namespace srose::gpu::opengl3
{
    Renderer::Renderer(SR_WM_display* disp)
        : gpu::Renderer(disp)
    {
        SR_ASSERT_CTX();
        SDL_LogInfo(
            SDL_LOG_CATEGORY_APPLICATION,
            "[OpenGL3] Renderer Information:\n%s",
            Information().c_str()
        );
    }

    void Renderer::ClearScreen(const glm::vec4& color) noexcept
    {
        SR_ASSERT_CTX();
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Present()
    {
        SR_ASSERT_CTX();
        SDL_GL_SwapWindow(GetDisplay()->win);
    }

    std::string Renderer::Information()
    {
        SR_ASSERT_CTX();

        using namespace std::literals;
        std::string info;
        info += "Vendor: "s + (const char*)glGetString(GL_VENDOR) + '\n';
        info += "Renderer: "s + (const char*)glGetString(GL_RENDERER) + '\n';
        info += "Version: "s + (const char*)glGetString(GL_VERSION) + '\n';

        return std::move(info);
    }

    void Renderer::ClearScreen()
    {
        ClearScreen(GetCurrentColor());
    }

    void Renderer::ShowDemoWindow(bool* p_open)
    {
        auto& uimgr = *ui::GetUIManager();
        if(!m_demo_initialized)
        {
            uimgr.widget_tree.emplace_at(
                "srose.opengl3.demo",
                std::make_shared<OpenGL3DemoWindow>(true)
            );
            m_demo_initialized = true;
        }
        auto ptr = uimgr.widget_tree["srose.opengl3.demo"].get();
        static_cast<OpenGL3DemoWindow*>(ptr)->open = true;
        ptr->Update();
        if(p_open)
            *p_open = static_cast<OpenGL3DemoWindow*>(ptr)->open;
    }

    void Renderer::ReleaseUIData() noexcept
    {
        if(!m_demo_initialized)
            return;
        m_demo_initialized = false;
        ui::GetUIManager()->widget_tree.erase_at("srose.opengl3.demo");
    }

    Texture* Renderer::NewTexture()
    {
        return new Texture;
    }
} // namespace srose::gpu::opengl3
