/**
 * @file filebrowser.cpp
 * @author HenryAWE
 * @brief File browser base
 */

#include <imguisr.h>


namespace ImGuiSR
{
    FileBrowserBase::~FileBrowserBase() = default;

    FileBrowserBase::FileBrowserBase() = default;


    IFileBrowser::IFileBrowser() = default;

    IFileBrowser::~IFileBrowser() = default;

    void IFileBrowser::SetCallback(Callback callback)
    {
        m_callback.swap(callback);
    }
    void IFileBrowser::InvokeCallback()
    {
        if(m_callback)
            m_callback(*this);
    }
} // namespace ImGuiSR
