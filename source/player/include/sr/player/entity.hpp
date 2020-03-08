/**
 * @file entity.hpp
 * @author HenryAWE
 * @brief Entitiy management
 */

#ifndef SROSE_PLAYER_entity_hpp_
#define SROSE_PLAYER_entity_hpp_

#include <glm/vec2.hpp>
#include <sr/gpu/renderer.hpp>


namespace srose::player
{
    class Entity
    {
    public:
        Entity() = default;

        virtual ~Entity() = default;

        virtual void Update() {}
        virtual void Render(gpu::Renderer& ren) {}
        virtual void OnDestroy() {}

        virtual void SetPosition(glm::vec2 pos) { m_position = pos; }
        [[nodiscard]]
        constexpr glm::vec2 GetPosition() const noexcept { return m_position; }

        virtual void SetScale(glm::vec2 scale) { m_scale = scale; }
        [[nodiscard]]
        constexpr glm::vec2 GetScale() const noexcept { return m_scale; }

        virtual void SetRotation(float radians) { m_rotation = radians; }
        [[nodiscard]]
        constexpr float GetRotation() const noexcept { return m_rotation; }

    protected:
        glm::vec2 m_position = glm::vec2(0, 0);
        glm::vec2 m_scale = glm::vec2(1.0f, 1.0f);
        float m_rotation = 0.0f; // In radians!!!
    };

    class SpriteEntity : public Entity
    {
        gpu::Sprite m_sprite;
    public:
        void SetSprite(const gpu::Sprite& sp) { m_sprite = sp; }
        [[nodiscard]]
        constexpr const gpu::Sprite& GetSprite() const noexcept
        {
            return m_sprite;
        }

        void Render(gpu::Renderer& ren) override;
    };
} // namespace srose::player


#endif
