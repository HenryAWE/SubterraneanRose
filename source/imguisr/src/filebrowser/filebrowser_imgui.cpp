/**
 * @file filebrowser_imgui.cpp
 * @author HenryAWE
 * @brief ImGui file browser
 */

#include <imgui.h>
#include "filebrowser_imgui.hpp"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>


namespace ImGuiSR
{
    void IImGuiFileBrowser::UpdateCacheImpl(
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
    void IImGuiFileBrowser::UpdateCache()
    {
        UpdateCacheImpl(m_cache, m_current, m_filters, m_pick_folder);
        m_update_req = false;
    }

    IImGuiFileBrowser::IImGuiFileBrowser()
        : m_id("###filebrowser"),
        m_current(srose::filesystem::current_path()) {}

    void IImGuiFileBrowser::Show()
    {
        UpdateCache();
        m_visible = true;
        m_update_req = true;
    }
    void IImGuiFileBrowser::Update()
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

    void IImGuiFileBrowser::UpdateFileView()
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

    bool IImGuiFileBrowser::visible() const { return m_visible; }

    void IImGuiFileBrowser::SetTitle(const std::string& title) { m_id = title + "###filebrowser"; }
    void IImGuiFileBrowser::SetFolder(const srose::filesystem::path& folder)
    {
        m_current = folder;
    }

    void IImGuiFileBrowser::SetFilter(const std::vector<FilterSpec>& filters)
    {
        m_filters = filters;
    }
    void IImGuiFileBrowser::SetPickFolder(bool value)
    {
        m_pick_folder = value;
    }

    bool IImGuiFileBrowser::HasResult() const
    {
        return m_result.has_value();
    }
    std::optional<srose::filesystem::path> IImGuiFileBrowser::GetResult()
    {
        return m_result;
    }
    void IImGuiFileBrowser::ClearResult()
    {
        m_result.reset();
    }
} // namespace ImGuiSR
