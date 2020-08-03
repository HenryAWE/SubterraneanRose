/**
 * @file filebrowser_win32.hpp
 * @author HenryAWE
 * @brief Win32 file browser
 */

#ifndef SROSE_IMGUISR_FILEBROWSER_filebrowser_win32_hpp_
#define SROSE_IMGUISR_FILEBROWSER_filebrowser_win32_hpp_

#include <string>
#ifdef _WIN32
#   include <comdef.h>
#   include <atlcomcli.h>
#   include <ShObjIdl_core.h>
#   include <SDL_syswm.h>
#   undef min
#   undef max
#endif
#include <imguisr.h>


namespace ImGuiSR
{
    #ifdef _WIN32
    class INativeFileBrowser : public IFileBrowser
    {
        IFileOpenDialog* m_dialog;
        ::HWND m_owner = nullptr;

        void CreateComInstance();

        std::optional<srose::filesystem::path> m_result;

        struct FilterSpecWin32
        {
            std::wstring name;
            std::wstring pattern;

            FilterSpecWin32(const FilterSpec& filter);
        };

        std::vector<FilterSpecWin32> m_filters;
    public:
        INativeFileBrowser();

        ~INativeFileBrowser() noexcept;

        void Show() override;
        void Update() override;

        bool visible() const override;

        void SetTitle(const std::string& title) override;
        void SetFolder(const srose::filesystem::path& folder) override;

        void SetFilter(const std::vector<FilterSpec>& filters) override;
        void SetPickFolder(bool value) override;

        bool HasResult() const override;
        std::optional<srose::filesystem::path> GetResult() override;
        void ClearResult() override;
    };
    #endif
} // namespace ImGuiSR


#endif
