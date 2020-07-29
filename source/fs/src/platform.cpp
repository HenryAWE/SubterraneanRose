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
#else
#   include <vector>
#   include <sys/types.h>
#   include <pwd.h>
#   include <unistd.h>
#endif


namespace srose::filesystem
{
    #ifndef _WIN32
    static path GetHome()
    {
        int uid = getuid();
        const char* home = getenv("HOME");
        if(uid != 0 && home)
        {
            return home;
        }
        passwd* pw = nullptr;
        passwd pwd{};
        auto bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
        if(bufsize < 0)
            bufsize = 16384;
        std::vector<char> buffer(bufsize);
        int err = getpwuid_r(uid, &pwd, buffer.data(), buffer.size(), &pw);
        if(err)
            throw std::runtime_error("getpwuid_r() failed");
        const char* result = pw->pw_dir;
        if(!result)
            throw std::runtime_error("Use has no home directory");
        return result;
    }

    path GetLinuxFolder(const char* envname, const char* fallback)
    {
        const char* env = getenv(envname);
        if(env)
        {
            path result(env);
            if(result.is_relative())
            {
                throw std::runtime_error(
                    "Path: \"" +  result.u8string() + "\" ($" + std::string(envname) +") is relative"
                );
            }
            return std::move(result);
        }
        else
        {
            return GetHome() / fallback;
        }
    }
    #endif

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
        #elif defined(__APPLE__)
        #error TODO: OS X
        #else
        return GetLinuxFolder("XDG_DATA_HOME", ".local/share");
        #endif
    }
} // namespace srose::filesystem
