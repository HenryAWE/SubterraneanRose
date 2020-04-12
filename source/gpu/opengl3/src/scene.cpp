/**
 * @file scene.cpp
 * @author HenryAWE
 * @brief OpenGL3 scene renderer
 */

#include <glad/glad.h>
#include <sr/gpu/opengl3/scene.hpp>
#include <sr/gpu/opengl3/renderer.hpp>


namespace srose::gpu::opengl3
{
    Scene::Scene(glm::ivec2 size, Renderer* ren)
        : gpu::Scene(size, ren)
    {
        Generate();
    }

    Scene::~Scene() = default;

    void Scene::Render(bool to_texture)
    {
        if(to_texture)
            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, GetSize().x, GetSize().y);
        if(m_render_callback)
            m_render_callback();

        if(to_texture)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    const Texture& Scene::GetScreenTexture() const
    {
        return m_screen;
    }

    void Scene::Generate()
    {
        m_fbo.Generate();
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        m_screen.Generate();
        Texture::Description frametex_desc{};
        frametex_desc.s = Texture::Wrapping::CLAMP_TO_EDGE;
        frametex_desc.t = Texture::Wrapping::CLAMP_TO_EDGE;
        auto framesize = GetSize();
        m_screen.LoadFromCurrentFramebuffer({framesize.x, framesize.y}, frametex_desc);
        m_rbo.Generate();
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framesize.x, framesize.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("[OpenGL3] Incomplete framebuffer");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Renderer* Scene::GetRenderer() const noexcept
    {
        return static_cast<Renderer*>(gpu::Scene::GetRenderer());
    }
} // namespace srose::gpu::opengl3
