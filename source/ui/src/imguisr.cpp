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
#include <boost/tokenizer.hpp>
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

        struct FilterSpecWin32
        {
            std::wstring name;
            std::wstring pattern;

            FilterSpecWin32(const FilterSpec& filter)
                : name(L(filter.name))
            {
                using namespace boost;
                char_separator<char> sep(";");
                tokenizer<char_separator<char>> tok(filter.exts, sep);
                std::string u8pattern;
                for(auto& i : tok)
                {
                    u8pattern += "*." + i + ';';
                }
                if(!u8pattern.empty())
                    u8pattern = u8pattern.substr(0, u8pattern.length() - 1);
                pattern = L(u8pattern);
            }
        };

        std::vector<FilterSpecWin32> m_filters;
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

        void SetTitle(const std::string& title) override
        {
            if(::HRESULT hr =  m_dialog->SetTitle(L(title).c_str()); FAILED(hr))
                _com_raise_error(hr);
        }
        void SetFolder(const srose::filesystem::path& folder) override
        {
            if(!is_directory(folder))
                return;

            CComPtr<IShellItem> item;
            ::HRESULT hr = ::SHCreateItemFromParsingName(
                absolute(folder).wstring().c_str(),
                nullptr,
                IID_PPV_ARGS(&item)
            );
            if(FAILED(hr))
                _com_raise_error(hr);

            hr = m_dialog->SetFolder(item);
            if(FAILED(hr))
                _com_raise_error(hr);
        }

        void SetFilter(const std::vector<FilterSpec>& filters) override
        {
            using namespace std;

            vector<FilterSpecWin32> tmp;
            tmp.reserve(filters.size());

            copy(filters.begin(), filters.end(), back_inserter(tmp));
            tmp.swap(m_filters);

            vector<COMDLG_FILTERSPEC> specs;
            specs.reserve(filters.size() + 1);
            transform(
                m_filters.begin(), m_filters.end(),
                back_inserter(specs),
                [](const FilterSpecWin32& in)->COMDLG_FILTERSPEC
                {
                    COMDLG_FILTERSPEC out{};
                    out.pszName = in.name.c_str();
                    out.pszSpec = in.pattern.c_str();
                    return out;
                }
            );
            COMDLG_FILTERSPEC all;
            all.pszName = L"All";
            all.pszSpec = L"*.*";
            specs.push_back(all);

            ::HRESULT hr = m_dialog->SetFileTypes(
                static_cast<UINT>(specs.size()),
                specs.data()
            );
            if(FAILED(hr))
                _com_raise_error(hr);
        }

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
