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
#include <sr/ui/uimgr.hpp>
#include "filebrowser/filebrowser_imgui.hpp"
#include "filebrowser/filebrowser_win32.hpp"


namespace ImGuiSR
{
    std::shared_ptr<IFileBrowser> CreateIFileBrowser()
    {
        return std::make_shared<IImGuiFileBrowser>();
    }
    std::shared_ptr<IFileBrowser> CreateNativeIFileBrowser()
    {
        #ifdef _WIN32
        return std::make_shared<INativeFileBrowser>();
        #else
        return nullptr;
        #endif
    }
} // namespace ImGuiSR
