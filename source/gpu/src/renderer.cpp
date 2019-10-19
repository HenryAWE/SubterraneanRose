/**
 * @file renderer.cpp
 * @author HenryAWE
 * @brief Renderer
 */

#include "sr/gpu/renderer.hpp"


namespace srose::gpu
{
    Renderer::Renderer() = default;

    Renderer::~Renderer() noexcept = default;

    void Renderer::PushColor(const glm::vec4& color)
    {
        m_color_stack.push(color);
    }
    void Renderer::PopColor() noexcept
    {
        if(!m_color_stack.empty())
            m_color_stack.pop();
    }
    const glm::vec4& Renderer::GetCurrentColor() const noexcept
    {
        static const glm::vec4 zero{0.0f, 0.0f, 0.0f, 0.0f};
        if(m_color_stack.empty())
            return zero;
        else
            return m_color_stack.top();
    }
} // namespace srose::gpu
