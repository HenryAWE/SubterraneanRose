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

    bool Renderer::LoadTextureFromFile(std::string_view as, const filesystem::path& file)
    {
        std::shared_ptr<Texture> tex(CreateTexture());
        bool result = tex->LoadFromFile(file);
        if(result)
        {
            textures.emplace_at(as, std::move(tex));
        }

        return result;
    }
} // namespace srose::gpu
