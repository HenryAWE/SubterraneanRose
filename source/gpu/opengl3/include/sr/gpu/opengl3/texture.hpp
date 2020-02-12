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
#include <sr/gpu/image.hpp>


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

        bool LoadFromFile(const char* path);
        bool LoadFromFileEx(const char* path, const Description& desc = {});

        [[nodiscard]]
        /**
         * @brief Get the native handle of the texture
         * 
         * @return handle_type The handle refer to the texture
         */
        handle_type handle() const noexcept { return m_handle; }

        std::pair<int, int> size() const noexcept { return m_size; }

        [[nodiscard]]
        native_handle_type GetNativeHandle() const noexcept override { return (void*)(std::intptr_t)m_handle; }

        bool LoadDefaultTexture() override;
        bool LoadFromFile(const filesystem::path& file) override;
        bool LoadFromFileEx(const filesystem::path& file, const Description& desc = {}) override;

        GLenum TranslateDesc(Filter filter, bool mipmap) noexcept;
        GLenum TranslateDesc(Wrapping wrap) noexcept;

    private:
        handle_type m_handle = 0;
        std::pair<int, int> m_size = {0, 0};
    };
} // namespace srose::gpu::opengl3


#endif
