/**
 * @file imguisr.h
 * @author HenryAWE
 * @brief ImGui extensions
 */

#ifndef SROSE_WM_IMGUISR_imguisr_h_
#define SROSE_WM_IMGUISR_imguisr_h_

#include <functional>
#include <utility>
#include <optional>
#include <memory>
#include <imgui.h>
#include <sr/filesystem/filesystem.hpp>


namespace ImGuiSR
{
    /* RAII helper classes for ImGui
     * 
     * Usage:
     *  auto object = ImGuiSR::PushGuard<ImGuiSR_ObjectType>(args...);
     */

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
        ImGuiSR_ID,
        ImGuiSR_Window,
        ImGuiSR_MenuBar,
        ImGuiSR_Menu,
        ImGuiSR_TabBar,
        ImGuiSR_TabItem
    };

    template <ImGuiSRPushType type>
    class PushGuard;

    template <>
    class PushGuard<ImGuiSR_ID> : public PushGuardBase
    {
    public:
        template <typename... Args>
        PushGuard(Args&&... args) noexcept
            : PushGuardBase((ImGui::PushID(std::forward<Args>(args)...), true)) {}

        ~PushGuard() noexcept { ImGui::PopID(); }
    };

    template <>
    class PushGuard<ImGuiSR_Window> : public PushGuardBase
    {
    public:
        template <typename... Args>
        PushGuard(Args&&... args) noexcept
            : PushGuardBase(ImGui::Begin(std::forward<Args>(args)...)) {}

        ~PushGuard() noexcept { ImGui::End(); }
    };

    #define IMGUISR_PUSHGUARD_TEMPLATE(objname, open_func, close_func) \
    template <>\
    class PushGuard<objname> : public PushGuardBase\
    {\
    public:\
        template <typename... Args>\
        PushGuard(Args&&... args) noexcept\
            : PushGuardBase(open_func(std::forward<Args>(args)...)) {}\
        \
        ~PushGuard() noexcept { if(value)close_func(); }\
    };

    IMGUISR_PUSHGUARD_TEMPLATE(ImGuiSR_MenuBar, ImGui::BeginMenuBar, ImGui::EndMenuBar);
    IMGUISR_PUSHGUARD_TEMPLATE(ImGuiSR_Menu, ImGui::BeginMenu, ImGui::EndMenu);
    IMGUISR_PUSHGUARD_TEMPLATE(ImGuiSR_TabBar, ImGui::BeginTabBar, ImGui::EndTabBar);
    IMGUISR_PUSHGUARD_TEMPLATE(ImGuiSR_TabItem, ImGui::BeginTabItem, ImGui::EndTabItem);

    class FileBrowserBase : public std::enable_shared_from_this<FileBrowserBase>
    {
    public:
        virtual ~FileBrowserBase();

        virtual void Show() = 0;
        virtual void Update() = 0;

        virtual bool visible() const = 0;

        virtual void SetTitle(const std::string& title) = 0;
        virtual void SetFolder(const srose::filesystem::path& folder) = 0;

        struct FilterSpec
        {
            std::string name;
            std::string exts; // Separated by semicolon ';', e.g. "jpg;jpeg"

            FilterSpec(std::string_view name_, std::string_view exts_)
                : name(name_), exts(exts_) {}
            FilterSpec(const FilterSpec&) = default;
            FilterSpec(FilterSpec&&) = default;

            FilterSpec& operator=(const FilterSpec&) = default;
            FilterSpec& operator=(FilterSpec&&) = default;
        };
        virtual void SetFilter(const std::vector<FilterSpec>& filters) = 0;
        virtual void SetPickFolder(bool value = true) = 0;

        virtual bool HasResult() const = 0;
        virtual std::optional<srose::filesystem::path> GetResult() = 0;
        virtual void ClearResult() = 0;

    protected:
        FileBrowserBase();
    };

    class IFileBrowser : public FileBrowserBase
    {
    public:
        IFileBrowser();

        ~IFileBrowser();
    };

    std::shared_ptr<IFileBrowser> CreateIFileBrowser(bool native = true);
} // namespace ImGuiSR


#endif
