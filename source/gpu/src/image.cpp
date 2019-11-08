/**
 * @file image.cpp
 * @author HenryAWE
 * @brief Image
 */

#include <sr/gpu/image.hpp>


namespace srose::gpu
{
    ImVec2 Texture::GetSizeImVec2() const noexcept
    {
        auto sz = size();
        return ImVec2(
            static_cast<float>(sz.first),
            static_cast<float>(sz.second)
        );
    }
} // namespace srose::gpu
