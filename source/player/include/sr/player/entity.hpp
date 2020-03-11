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


    protected:
    };
} // namespace srose::player


#endif
