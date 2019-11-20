/**
 * @file imguisr.h
 * @author HenryAWE
 * @brief ImGui extensions
 */

#ifndef SROSE_WM_IMGUISR_imguisr_h_
#define SROSE_WM_IMGUISR_imguisr_h_

#include <utility>
#include <imgui.h>


namespace ImGuiSR
{
    class PushGuardBase
    {
    public:
        const bool value;

        [[nodiscard]]
        constexpr operator bool() const noexcept { return value; }
    
    protected:
        PushGuardBase(bool value_) noexcept
            : value(value_) {}
    };

    enum ImGuiSRPushType : int
    {
        ImGuiSR_Window
    };

    template <ImGuiSRPushType type>
    class PushGuard;

    template <>
    class PushGuard<ImGuiSR_Window> : public PushGuardBase
    {
    public:
        template <typename... Args>
        PushGuard(Args&&... args) noexcept
            : PushGuardBase(ImGui::Begin(std::forward<Args>(args)...)) {}

        ~PushGuard() noexcept { ImGui::End(); }
    };
} // namespace ImGuiSR


#endif
