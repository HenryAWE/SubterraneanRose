/**
 * @file filebrowser_win32.cpp
 * @author HenryAWE
 * @brief Win32 file browser
 */

#include "filebrowser_win32.hpp"
#include <boost/locale.hpp>
#include <boost/tokenizer.hpp>
#include <sr/ui/uimgr.hpp>


namespace ImGuiSR
{
#ifdef _WIN32
        /* Helper */
        static std::wstring L(const std::string& str)
        {
            return boost::locale::conv::utf_to_utf<wchar_t>(str);
        }

        void INativeFileBrowser::CreateComInstance()
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

        INativeFileBrowser::FilterSpecWin32::FilterSpecWin32(const FilterSpec& filter)
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

        INativeFileBrowser::INativeFileBrowser()
        {
            CreateComInstance();
            SDL_SysWMinfo info{};
            SDL_GetWindowWMInfo(
                srose::ui::UIManager::GetInstance().GetWindow().handle(),
                &info
            );
            m_owner = info.info.win.window;
        }

        INativeFileBrowser::~INativeFileBrowser() noexcept
        {
            if(m_dialog)
                m_dialog->Release();
            m_dialog = nullptr;
        }

        void INativeFileBrowser::Show()
        {
            m_result.reset();
            if(::HRESULT hr = m_dialog->Show(m_owner); FAILED(hr))
            {
                if(hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
                    _com_raise_error(hr);

                m_result.reset();
                m_callback_invoked = false;
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
            m_callback_invoked = false;
        }
        void INativeFileBrowser::Update()
        {
            if(!m_callback_invoked)
            {
                m_callback_invoked = true;
                InvokeCallback();
            }
        }

        bool INativeFileBrowser::visible() const
        {
            return false;
        }

        void INativeFileBrowser::SetTitle(const std::string& title)
        {
            if(::HRESULT hr =  m_dialog->SetTitle(L(title).c_str()); FAILED(hr))
                _com_raise_error(hr);
        }
        void INativeFileBrowser::SetFolder(const srose::filesystem::path& folder)
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

        void INativeFileBrowser::SetFilter(const std::vector<FilterSpec>& filters)
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
            if(FAILED(hr) && hr != E_UNEXPECTED)
                _com_raise_error(hr);
        }
        void INativeFileBrowser::SetPickFolder(bool value)
        {
            ::FILEOPENDIALOGOPTIONS options;
            ::HRESULT hr = m_dialog->GetOptions(&options);
            if(FAILED(hr))
                _com_raise_error(hr);

            if(value)
                options |= FOS_PICKFOLDERS;
            else
                options &= ~FOS_PICKFOLDERS;

            hr = m_dialog->SetOptions(options);
            if(FAILED(hr))
                _com_raise_error(hr);
        }

        bool INativeFileBrowser::HasResult() const
        {
            return m_result.has_value();
        }
        std::optional<srose::filesystem::path> INativeFileBrowser::GetResult()
        {
            return m_result;
        }
        void INativeFileBrowser::ClearResult()
        {
            m_result.reset();
        }
#endif
} // namespace ImGuiSR
