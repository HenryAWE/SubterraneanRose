/**
 * @file platform.cpp
 * @author HenryAWE
 * @brief Platform specific folders
 */

#include <sr/filesystem/platform.hpp>
#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#   include <ShlObj.h>
#endif


namespace srose::filesystem
{
    path GetSysAppData()
    {
        #ifdef _WIN32
        ::LPWSTR appdata = nullptr;
        ::HRESULT hr = ::SHGetKnownFolderPath(
            FOLDERID_RoamingAppData,
            KF_FLAG_CREATE,
            nullptr,
            &appdata
        );
        if(FAILED(hr))
        {
            ::CoTaskMemFree(appdata);
            throw std::runtime_error("FOLDERID_RoamingAppData could not be found");
        }
        path result(appdata);
        ::CoTaskMemFree(appdata);
        return std::move(result);
        #else
        //TODO: Linux & OS X
        return "~";
        #endif
    }
} // namespace srose::filesystem
