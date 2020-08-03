/**
 * @file filebrowser_imgui.hpp
 * @author HenryAWE
 * @brief ImGui file browser
 */

#ifndef SROSE_IMGUISR_FILEBROWSER_filebrowser_imgui_hpp_
#define SROSE_IMGUISR_FILEBROWSER_filebrowser_imgui_hpp_

#include <string>
#include <optional>
#include <imguisr.h>
#include <sr/filesystem/filesystem.hpp>


namespace ImGuiSR
{
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
        );
        void UpdateCache();

    public:
        IImGuiFileBrowser();

        void Show() override;
        void Update() override;

        void UpdateFileView();

        bool visible() const override;

        void SetTitle(const std::string& title) override;
        void SetFolder(const srose::filesystem::path& folder) override;

        void SetFilter(const std::vector<FilterSpec>& filters) override;
        void SetPickFolder(bool value) override;

        bool HasResult() const override;
        std::optional<srose::filesystem::path> GetResult() override;
        void ClearResult() override;
    };
} // namespace ImGuiSR


#endif
