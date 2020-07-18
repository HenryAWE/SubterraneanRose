/**
 * @file about.cpp
 * @author HenryAWE
 * @brief About page
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "about.hpp"
#include <cassert>
#include <imgui.h>
#include <imgui_internal.h>
#include <sr/ui/uimgr.hpp>
#include <sr/util/shell.hpp>


namespace srose::ui
{
    About::About()
        : Base("about")
    {
        AddString("return", "srose.ui.common.return");
        AddString("website", "srose.ui.about.website");
    }

    void About::Update()
    {
        Base::Update();

        auto& io = ImGui::GetIO();

        SetFlags(ImGuiWindowFlags_NoScrollbar);
        auto background = BeginContext();

        if(ImGui::Button(GetString("return").c_str()))
        {
            UIManager::GetInstance().PopRootNode();
        }
        if(ImGui::Button(GetString("website").c_str()))
        {
            util::OpenInBrowser("https://github.com/HenryAWE/SubterraneanRose");
        }
    }

    void About::LoadI18nData()
    {
        
    }
} // namespace srose::ui
