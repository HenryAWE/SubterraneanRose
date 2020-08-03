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
#include "filebrowser/filebrowser_imgui.hpp"
#include "filebrowser/filebrowser_win32.hpp"


namespace ImGuiSR
{
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
