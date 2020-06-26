/**
 * @file srlc_editor.cpp
 * @author HenryAWE
 * @brief SRLC editor
 */

#define IMGUI_DEFINE_MATH_OPERATORS
#include "srlc_editor.hpp"
#include <imguisr.h>
#include <imgui_internal.h>
#include <sr/locale/locale.hpp>


namespace srose::ui::editor
{
    SrlcEditor::SrlcEditor()
        : Base("", "srose.srlc-editor")
    {
        SetId(gettext("srose.ui.srlc-editor"));
        m_tree = getptr()->GetStringTree();
    }

    SrlcEditor::~SrlcEditor() = default;

    void SrlcEditor::Update()
    {
        auto& io = ImGui::GetIO();

        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
        auto window =  BeginContext();
        if(!window)
            return;

        IterateTree(m_tree);
    }

    void SrlcEditor::LoadI18nData()
    {
        SetId(gettext("srose.ui.srlc-editor"));
        m_tree = getptr()->GetStringTree();
    }

    void SrlcEditor::IterateTree(util::string_tree<std::string>& tr)
    {
        if(tr.has_value())
            ImGui::BulletText("%s", (*tr).c_str());
        for(auto& i : tr)
        {
            if(!ImGui::TreeNode(i.first.c_str()))
                continue;
            IterateTree(i.second);
            ImGui::TreePop();
        }
    }
} // namespace srose::ui::editor
