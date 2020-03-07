/**
 * @file sprite.hpp
 * @author HenryAWE
 * @brief Sprite
 */

#ifndef SROSE_GPU_OPENGL3_sprite_hpp_
#define SROSE_GPU_OPENGL3_sprite_hpp_

#include <glm/glm.hpp>
#include "image.hpp"


namespace srose::gpu
{
    class Sprite
    {
    public:
        void SetTexture(Texture* tex)
        {
            m_texture = tex;
        }
        [[nodiscard]]
        Texture* GetTexture() const noexcept
        {
            return m_texture;
        }

        void position(glm::vec2 pos) noexcept
        {
            m_position = pos;
        }
        [[nodiscard]]
        glm::vec2 position() const noexcept
        {
            return m_position;
        }

        void scale(glm::vec2 value) noexcept
        {
            m_scale = value;
        }
        [[nodiscard]]
        glm::vec2 scale() const noexcept
        {
            return m_scale;
        }

        constexpr void rotation_radians(float rad) noexcept
        {
            m_rotation = rad;
        }
        [[nodiscard]]
        constexpr float rotation_radians() const noexcept
        {
            return m_rotation;
        }
        constexpr void rotation(float deg) noexcept
        {
            m_rotation = glm::radians(deg);
        }
        [[nodiscard]]
        constexpr float rotation() const noexcept
        {
            return glm::degrees(m_rotation);
        }

        glm::mat4 CalcMatrix() const noexcept;

    private:
        glm::vec2 m_position = glm::vec2(0.0f);
        glm::vec2 m_scale = glm::vec2(1.0f);
        float m_rotation = 0.0f; // In radians, not in degrees!
        Texture* m_texture = nullptr;
    };
} // namespace srose::gpu


#endif
