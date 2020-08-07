/**
 * @file system.cpp
 * @author HenryAW
 * @brief System
 */

#define __STDC_WANT_LIB_EXT1__ 1
#include <sr/util/system.hpp>
#include <stdlib.h>
#include <boost/locale.hpp>


namespace srose::util
{
    std::optional<std::string> GetEnv(const std::string& name)
    {
        using namespace std;

        // Use wide character version provided by MSVC to avoid encoding issue
        #if defined(_WIN32) && defined(_MSC_VER)
        using boost::locale::conv::utf_to_utf;
        wstring wname = utf_to_utf<wchar_t>(name);
        size_t wenvvar_len = 0;
        // Use the "safe" version to avoid the C4996
        if(_wgetenv_s(&wenvvar_len, nullptr, 0, wname.c_str()) == 0 && wenvvar_len)
        {
            wstring wenvvar;
            // The "wenvvar_len" includes the '\0' at the end of the string
            wenvvar.resize(wenvvar_len - 1);
            _wgetenv_s(&wenvvar_len, wenvvar.data(), wenvvar.size() + 1, wname.c_str());

            return utf_to_utf<char>(wenvvar);
        }
        else
        {
            return nullopt;
        }
        #else // Normal version
        const char* envvar = getenv(name.c_str());
        return envvar ? optional<string>(envvar) : nullopt;
        #endif
    }
} // namespace srose::util
