/**
 * @file impl.cpp
 * @author HenryAWE
 * @brief Editor implementation
 */

#include <imgui.h>
#include "impl.hpp"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>


namespace srose::ui::editor
{
    EditorImpl::EditorImpl()
    {
        if(m_use_native_file_dialog)
            m_file_open_dialog = ImGuiSR::CreateNativeIFileBrowser();
        else
            m_file_open_dialog = ImGuiSR::CreateIFileBrowser();
    }

    EditorImpl::~EditorImpl() = default;

    void EditorImpl::Update()
    {
        m_file_open_dialog->Update();
    }

    bool EditorImpl::UseNativeFileDialog()
    {
        return m_use_native_file_dialog;
    }
    void EditorImpl::UseNativeFileDialog(bool use)
    {
        if(use == m_use_native_file_dialog)
            return;

        m_use_native_file_dialog = use;
        if(m_use_native_file_dialog)
            m_file_open_dialog = ImGuiSR::CreateNativeIFileBrowser();
        else
            m_file_open_dialog = ImGuiSR::CreateIFileBrowser();
    }

    ImGuiSR::IFileBrowser& EditorImpl::GetFileOpenDialog()
    {
        return *m_file_open_dialog;
    }
} // namespace srose::ui::editor
