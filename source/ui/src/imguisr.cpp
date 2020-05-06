/**
 * @file imguisr.cpp
 * @author HenryAWE
 * @brief ImGui extensions
 */

#include <imguisr.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>
#include <SDL.h>
#include <boost/locale.hpp>
#ifdef __WINDOWS__
#   include <comdef.h>
#   include <atlcomcli.h>
#   include <ShObjIdl_core.h>
#   include <SDL_syswm.h>
#endif
#include <sr/ui/uimgr.hpp>


namespace ImGuiSR
{
    FileBrowserBase::~FileBrowserBase() = default;

    FileBrowserBase::FileBrowserBase() = default;


    IFileBrowser::IFileBrowser() = default;

    IFileBrowser::~IFileBrowser() = default;


#ifdef __WINDOWS__
    class INativeFileBrowser : public IFileBrowser
    {
        IFileOpenDialog* m_dialog;
        ::HWND m_owner = nullptr;

        void CreateComInstance()
        {
            ::HRESULT hr = ::CoCreateInstance(
                CLSID_FileOpenDialog,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_dialog)
            );
            if(FAILED(hr))
                _com_raise_error(hr);
        }

        /* Helper */
        static std::wstring L(const std::string& str)
        {
            return boost::locale::conv::utf_to_utf<wchar_t>(str);
        }

        std::optional<srose::filesystem::path> m_result;

    public:
        INativeFileBrowser()
        {
            CreateComInstance();
            SDL_SysWMinfo info{};
            SDL_GetWindowWMInfo(
                srose::ui::UIManager::GetInstance().GetWindow().handle(),
                &info
            );
            m_owner = info.info.win.window;
        }

        ~INativeFileBrowser() noexcept
        {
            if(m_dialog)
                m_dialog->Release();
            m_dialog = nullptr;
        }

        void Show() override
        {
            m_result.reset();
            if(::HRESULT hr = m_dialog->Show(m_owner); FAILED(hr))
            {
                if(hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
                    _com_raise_error(hr);
                return;
            }
            CComPtr<IShellItem> item;
            if(::HRESULT hr = m_dialog->GetResult(&item); FAILED(hr))
                _com_raise_error(hr);
            wchar_t* name = nullptr;
            if(::HRESULT hr = item->GetDisplayName(SIGDN_FILESYSPATH, &name); FAILED(hr))
                _com_raise_error(hr);
            using guard_t = std::unique_ptr<wchar_t, std::function<void(wchar_t*)>>;
            guard_t guard(name, [](wchar_t* p){ ::CoTaskMemFree(p); });
            m_result = name;
        }
        void Update() override {}

        bool visible() const override { return false; }

        std::optional<srose::filesystem::path> GetResult() override
        {
            return m_result;
        }
    };
#endif

    std::shared_ptr<IFileBrowser> CreateIFileBrowser(bool native)
    {
    #ifdef __WINDOWS__
        return std::make_shared<INativeFileBrowser>();
    #else
        return nullptr;
    #endif
    }
} // namespace ImGuiSR
