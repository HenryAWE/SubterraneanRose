/**
 * @file impl.hpp
 * @author HenryAWE
 * @brief Editor implementation
 */

#ifndef SROSE_UI_EDITOR_impl_hpp_
#define SROSE_UI_EDITOR_impl_hpp_

#include <memory>
#include <imguisr.h>


namespace srose::ui::editor
{
    class EditorImpl
    {
        bool m_use_native_file_dialog = false;
        std::shared_ptr<ImGuiSR::IFileBrowser> m_file_open_dialog;
    public:
        EditorImpl();

        ~EditorImpl();

        void Update();

        [[nodiscard]]
        bool UseNativeFileDialog(); // get
        void UseNativeFileDialog(bool use); // set
        ImGuiSR::IFileBrowser& GetFileOpenDialog();
    };
} // namespace srose::ui::editor


#endif
