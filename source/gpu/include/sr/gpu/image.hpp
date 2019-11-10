/**
 * @file image.hpp
 * @author HenryAWE
 * @brief Image
 */

#ifndef SROSE_GPU_image_hpp_
#define SROSE_GPU_image_hpp_

#include <utility>
#include <imgui.h>
#include <sr/filesystem/filesystem.hpp>


namespace srose::gpu
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
         * @return std::pair<int, int> Texture size
         */
        virtual std::pair<int, int> size() const noexcept = 0;

        [[nodiscard]]
        /**
         * @brief Get the texture size for ImGui
         * 
         * @return ImVec2 Size in pixels
         */
        ImVec2 GetSizeImVec2() const noexcept;

        /**
         * @brief Load a texture from file
         * 
         * @param file The texture file
         * @return true Succeeded
         * @return false Failed
         */
        virtual bool LoadFromFile(const filesystem::path& file) = 0;
    };
} // namespace srose::gpu


#endif
