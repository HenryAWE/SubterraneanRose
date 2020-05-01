/**
 * @file demo.cpp
 * @author HenryAWE
 * @brief Player demo
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "demo.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include <sr/gpu/renderer.hpp>
#include <sr/player/component/transform.hpp>
#include <sr/player/component/move.hpp>
#include <sr/player/component/image.hpp>
#include <sr/player/system/system.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::player
{
    struct DemoCounter : public component::Component<DemoCounter>
    {
        DemoCounter() noexcept {}
        std::size_t counter = 0;
    };
    struct DemoSystem
    {
        typedef std::tuple<DemoCounter, component::Transform> Components;

        void Update(entity::Entity e, DemoCounter& t, component::Transform& tr)
        {
            float val = float(t.counter++) / 10.0f / 3.1415926f;
            tr.position = glm::vec2(350/2, 430/2);
            tr.position += 80.0f * glm::vec2(std::cos(val), std::sin(val));
            tr.rotation += glm::radians(30.0f);
        }
    };

    PlayerDemoWindow::PlayerDemoWindow(gpu::Renderer& ren)
        : m_ren(&ren), m_stage({350, 430})
    {
        auto& emgr = m_stage.world.GetEntityManager();
        auto id = emgr.CreateEntity().GetId();
        emgr.AssignComponent<component::Image>(id, nullptr);
        emgr.AssignComponent<component::Transform>(id, glm::vec2(350/2, 430/2), glm::vec2(10));
        emgr.AssignComponent<DemoCounter>(id);

        m_stage.world.GetSystemManager(m_stage.world.UPDATE)
            .Add<DemoSystem, DemoSystem::Components>();

        for(int i = 0; i < 12; ++i)
        {
            auto id = emgr.CreateEntity().GetId();
            emgr.AssignComponent<component::Image>(id, nullptr);
            glm::vec2 position = glm::vec2(350/2, 430/2);
            float val = i * 3.1415926f / 4.0f;
            position.x += 100.0f * std::cos(val);
            position.y += 100.0f * std::sin(val);

            emgr.AssignComponent<component::Transform>(id, position, glm::vec2(8.5f));
        }

        m_ren->AddRenderData(m_stage);
    }

    void PlayerDemoWindow::Update()
    {
        m_stage.world.Update();
        auto& io = ImGui::GetIO();

        constexpr int background_flags = 
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "Player Demo",
            &open,
            background_flags
        );
        if(!background)
            return;

        auto& screen = *m_stage.world.GetGlobalComponent<gpu::StageRenderData>()->screen_texture;
        if(ImGui::BeginChild("##screen"))
        {
            ImGui::Image(
                screen.GetNativeHandle(),
                screen.GetSizeImVec2(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }
        ImGui::EndChild();
    }

    void PlayerDemoWindow::Render()
    {
        m_ren->RenderStage(m_stage);
    }
} // namespace srose::player

#endif
