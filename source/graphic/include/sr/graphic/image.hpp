/**
 * @file image.hpp
 * @author HenryAWE
 * @brief Image
 */

#ifndef SROSE_GRAPHIC_image_hpp_
#define SROSE_GRAPHIC_image_hpp_

#include <utility>
#include <imgui.h>
#include <sr/filesystem/filesystem.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace srose::graphic
{
    class Texture
    {
    public:
        typedef void* native_handle_type;

        Texture() = default;

        virtual ~Texture() noexcept = default;

        [[nodiscard]]
        /**
         * @brief Get the native handle pointer
         * @remark This function is used to work with ImGui
         * 
         * @return native_handle_type Native handle
         */
        virtual native_handle_type GetNativeHandle() const noexcept = 0;

        [[nodiscard]]
        /**
         * @brief Get the texture size in pixels
         * 
         * @return glm::ivec2 Texture size
         */
        virtual glm::ivec2 size() const noexcept = 0;

        [[nodiscard]]
        /**
         * @brief Get the texture size for ImGui
         * 
         * @return ImVec2 Size in pixels
         */
        ImVec2 GetSizeImVec2() const noexcept;

        /**
         * @brief Texture wrapping method
         */
        enum Wrapping : int
        {
            REPEAT,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER
        };
        /**
         * @brief Texture filter
         */
        enum Filter : int
        {
            LINEAR,
            NEAREST,
        };
        /**
         * @brief Texture description
         */
        struct Description
        {
            Wrapping s = REPEAT;
            Wrapping t = REPEAT;
            glm::vec4 border_color = {0, 0, 0, 0};
            Filter min = LINEAR;
            Filter mag = LINEAR;
        };

        /**
         * @brief Load a default red and black texture
         * 
         * @return bool Always true
         */
        virtual bool LoadDefaultTexture() = 0;
        /**
         * @brief Load a texture from file
         * 
         * @param file The texture file
         * @return true Succeeded
         * @return false Failed
         */
        virtual bool LoadFromFile(const filesystem::path& file) = 0;
        /**
         * @brief [Advanced] Load a texture from file
         * 
         * @param file The texture file
         * @param desc Texture description
         * @return true Succeeded
         * @return false Failed
         */
        virtual bool LoadFromFileEx(const filesystem::path& file, const Description& desc) = 0;
    };
} // namespace srose::graphic


#endif
