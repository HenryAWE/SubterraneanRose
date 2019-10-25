/**
 * @file fontfile.cpp
 * @author HenryAWE
 * @brief Locating font file
 */

#include <sr/filesystem/fontfile.hpp>
#include <cctype>
#include <string.h>


namespace srose::filesystem
{
    static bool MatchFontFileExt(std::string_view ext)
    {
        return
            ext==".ttf"||ext==".TTF"||
            ext==".ttc"||ext==".TTC";
    }
    static bool MatchFontFileExt(std::wstring_view ext)
    {
        return
            ext==L".ttf"||ext==L".TTF"||
            ext==L".ttc"||ext==L".TTC";
    }

    static std::optional<path> IterateDirectory(const std::string& name, const path& location)
    {
        if(!is_directory(location))
            return std::nullopt;
        for(auto dt : recursive_directory_iterator(location))
        {
            auto pt = dt.path();
            if(!is_regular_file(pt) || !pt.has_stem())
                continue;
            if(
                pt.stem().string()==name&&
                MatchFontFileExt(pt.extension().c_str())
            ) {
                return pt;
            }
        }

        return std::nullopt;
    }

    std::optional<path> GetFont(const std::string& name)
    {
        auto pt = IterateDirectory(name, "resource/fonts");
        if(pt.has_value()) return std::move(pt);
    #ifdef __linux__
        pt = IterateDirectory(name, "~/.fonts/");
        if(pt.has_value()) return std::move(pt);
        pt = IterateDirectory(name, "/usr/share/fonts/");
        if(pt.has_value()) return std::move(pt);
    #endif

        return std::nullopt;
    }
} // namespace srose::filesystem
