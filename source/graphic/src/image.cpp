/**
 * @file image.cpp
 * @author HenryAWE
 * @brief Image
 */

#include <sr/graphic/image.hpp>


namespace srose::graphic
{
    ImVec2 Texture::GetSizeImVec2() const noexcept
    {
        auto sz = size();
        return ImVec2(
            static_cast<float>(sz.x),
            static_cast<float>(sz.y)
        );
    }
} // namespace srose::graphic
