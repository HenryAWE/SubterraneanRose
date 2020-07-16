/**
 * @file texture.hpp
 * @author HenryAWE
 * @brief OpenGL texture wrapper
 */

#ifndef SROSE_GPU_OPENGL3_texture_hpp_
#define SROSE_GPU_OPENGL3_texture_hpp_

#include <glad/glad.h>
#include <filesystem>
#include <utility>
#include <glm/vec2.hpp>
#include <sr/gpu/image.hpp>
#include "buffer.hpp"


namespace srose::gpu::opengl3
{
    /**
     * @brief OpenGL texture wrapper
     */
    class Texture : public gpu::Texture
    {
    public:
        typedef GLuint handle_type;

        ~Texture() noexcept;

        void Generate() noexcept;
        void Destroy() noexcept;

        bool LoadFromFile(const char* path, bool mipmap = true);
        bool LoadFromFileEx(const char* path, const Description& desc = {}, bool mipmap = true);
        bool LoadFromCurrentFramebuffer(std::pair<int, int> framesize, const Description& desc = {}, bool mipmap = false);

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }

        glm::ivec2 size() const noexcept { return m_size; }

        [[nodiscard]]
        native_handle_type GetNativeHandle() const noexcept override { return (void*)(std::intptr_t)m_handle; }

        bool LoadDefaultTexture() override;
        bool LoadFromFile(const filesystem::path& file) override;
        bool LoadFromFileEx(const filesystem::path& file, const Description& desc = {}) override;

        static GLenum TranslateDesc(Filter filter, bool mipmap) noexcept;
        static GLenum TranslateDesc(Wrapping wrap) noexcept;

    protected:
        static void ApplyDesc(const Description& desc, bool mipmap = true) noexcept;

        handle_type m_handle = 0;
        glm::ivec2 m_size{0, 0};
    };


    /**
     * @brief Screen texture wrapper
     */
    class ScreenTexture : public Texture
    {
        typedef Texture base;

        // Mark some members of the base class as private
        using base::LoadDefaultTexture;
        using base::LoadFromFile;
        using base::LoadFromFileEx;
        using base::TranslateDesc;

    public:
        void Generate(glm::ivec2 size);
        void Destroy();

        [[nodiscard]]
        constexpr Framebuffer& framebuffer() noexcept { return m_fbo; }
        [[nodiscard]]
        constexpr const Framebuffer& framebuffer() const noexcept { return m_fbo; }

        [[nodiscard]]
        constexpr Renderbuffer& renderbuffer() noexcept { return m_rbo; }
        [[nodiscard]]
        constexpr const Renderbuffer& renderbuffer() const noexcept { return m_rbo; }

    private:
        Framebuffer m_fbo;
        Renderbuffer m_rbo;
    };
} // namespace srose::gpu::opengl3


#endif
