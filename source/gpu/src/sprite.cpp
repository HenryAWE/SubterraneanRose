/**
 * @file sprite.cpp
 * @author HenryAWE
 * @brief OpenGL sprite
 */

#include <sr/gpu/sprite.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace srose::gpu::opengl3
{
    glm::mat4 Sprite::CalcMatrix() const noexcept
    {
        glm::mat4 result = glm::mat4(1);
        result = glm::translate(result, glm::vec3(m_position, 0.0f));
        result = glm::scale(result, glm::vec3(m_scale, 1.0f));
        result = glm::rotate(result, m_rotation, glm::vec3(0.0f, 0.0f, -1.0f));

        return result;
    }
} // namespace srose::gpu::opengl3
