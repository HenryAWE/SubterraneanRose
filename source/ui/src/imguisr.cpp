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
#   undef min
#   undef max
#endif
#include <sr/ui/uimgr.hpp>


namespace ImGuiSR
{
    FileBrowserBase::~FileBrowserBase() = default;

    FileBrowserBase::FileBrowserBase() = default;


    IFileBrowser::IFileBrowser() = default;

    IFileBrowser::~IFileBrowser() = default;


    class IImGuiFileBrowser : public IFileBrowser
    {
        std::string m_id;
        bool m_visible = false;

        srose::filesystem::path m_current;
        std::vector<FilterSpec> m_filters;
        bool m_pick_folder = false;

        std::optional<srose::filesystem::path> m_result;

        struct Cache
        {
            std::vector<std::string> path_elements;
            typedef std::tuple<
                srose::filesystem::path, // filename
                std::string, // name (with/without extension)
                bool // is folder
            > FileInfo;
            std::vector<FileInfo> files;
        };
        Cache m_cache;
        bool m_update_req = false;

        static void UpdateCacheImpl(
            Cache& cache,
            const srose::filesystem::path& folder,
            const std::vector<FilterSpec>& filters,
            bool pick_folder
        ) {
                assert(is_directory(folder));

                namespace fs = srose::filesystem;

                cache.path_elements.clear();
                std::transform(
                    folder.begin(), folder.end(),
                    std::back_inserter(cache.path_elements),
                    [](const fs::path& p) { return p.u8string(); }
                );

                cache.files.clear();
                for(auto dt : fs::directory_iterator(folder))
                {
                    using std::get;

                    auto pt = dt.path();
                    Cache::FileInfo info;
                    get<0>(info) = pt.filename();
                    get<1>(info) = pt.stem().u8string();
                    get<2>(info) = is_directory(pt);

                    cache.files.push_back(std::move(info));
                }
        }
        void UpdateCache()
        {
            UpdateCacheImpl(m_cache, m_current, m_filters, m_pick_folder);
            m_update_req = false;
        }
    public:
        IImGuiFileBrowser()
            : m_id("###filebrowser"),
            m_current(srose::filesystem::current_path()) {}

        void Show() override
        {
            UpdateCache();
            m_visible = true;
            m_update_req = true;
        }
        void Update() override
        {
            if(!m_visible)
                return;

            if(m_update_req)
                UpdateCache();

            auto& io = ImGui::GetIO();

            auto clsid = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID>(this);

            constexpr int popup_flags =
                ImGuiWindowFlags_MenuBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoSavedSettings;

            ImGui::OpenPopup(m_id.c_str());
            ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
            ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
            if(!ImGui::BeginPopupModal(m_id.c_str(), &m_visible, popup_flags))
            {
                return;
            }
            if(ImGui::Button("Current"))
            {
                m_current = srose::filesystem::current_path(),
                UpdateCache();
            }
            ImGui::SameLine();
            ImGui::Separator();

            std::size_t idx = 0;
            bool idx_pressed = false;
            for(auto& i : m_cache.path_elements)
            {
                ImGui::SameLine();
                idx_pressed |= ImGui::Button(i.c_str());
                if(!idx_pressed) ++idx;
            }

            if(!m_cache.path_elements.empty() && idx_pressed)
            {
                if(idx <= 1)
                    idx = std::min((std::size_t)1, m_cache.path_elements.size());

                srose::filesystem::path tmp;
                for(std::size_t i = 0; i < idx + 1; ++i)
                {
                    tmp.append(m_cache.path_elements[i]);
                }
                tmp.swap(m_current);
                m_update_req = true;
            }

            ImGui::Separator();

            ImGui::BeginChild("##fileview");
            UpdateFileView();
            ImGui::EndChild();

            ImGui::EndPopup();
        }

        void UpdateFileView()
        {
            for(auto& i : m_cache.files)
            {
                using std::get;

                if(ImGui::MenuItem(get<1>(i).c_str(), get<2>(i)?"Folder":""))
                {
                    if(get<2>(i))
                    {
                        m_current /= get<0>(i);
                        m_update_req = true;
                    }
                    else
                    {
                        m_result = m_current / get<0>(i);
                        m_visible = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }

        bool visible() const override { return m_visible; }

        void SetTitle(const std::string& title) override { m_id = title + "###filebrowser"; }
        void SetFolder(const srose::filesystem::path& folder) override
        {
            m_current = folder;
        }

        void SetFilter(const std::vector<FilterSpec>& filters) override
        {
            m_filters = filters;
        }
        void SetPickFolder(bool value) override { m_pick_folder = value; }

        std::optional<srose::filesystem::path> GetResult() override
        {
            return m_result;
        }
    };


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
            if(FAILED(hr) && hr != E_UNEXPECTED)
                _com_raise_error(hr);
        }
        void SetPickFolder(bool value) override
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

        std::optional<srose::filesystem::path> GetResult() override
        {
            return m_result;
        }
    };
#endif

    std::shared_ptr<IFileBrowser> CreateIFileBrowser(bool native)
    {
    #ifdef __WINDOWS__
        if(native)
            return std::make_shared<INativeFileBrowser>();
        else
            return std::make_shared<IImGuiFileBrowser>();
    #else
        return std::make_shared<IImGuiFileBrowser>();
    #endif
    }
} // namespace ImGuiSR
