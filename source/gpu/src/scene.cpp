/**
 * @file scene.cpp
 * @author HenryAWE
 * @brief Scene
 */

#include <sr/gpu/scene.hpp>


namespace srose::gpu
{
    Scene::Scene(glm::ivec2 size)
        : m_size(size) {}

    Scene::~Scene() = default;

    void Scene::SetRenderCallback(std::function<callback_type> func)
    {
        m_render_callback = std::move(func);
    }
} // namespace srose::gpu
