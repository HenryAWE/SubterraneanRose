/**
 * @file imgui_ini.hpp
 * @author HenryAWE
 * @brief Handling imgui.ini
 */

#ifndef SROSE_IMGUISR_IMGUI_INI_HPP_
#define SROSE_IMGUISR_IMGUI_INI_HPP_

#include <filesystem>
#include <vector>
#include <imgui.h>


namespace ImGuiSR
{
    typedef std::vector<char> ImGuiIniData;

    std::filesystem::path GetIniDataFile();

    void SaveIniDataToDisk(
        const ImGuiIniData& ini,
        const std::filesystem::path file = GetIniDataFile()
    );
    void LoadIniDataFromDisk(
        ImGuiIniData& ini,
        const std::filesystem::path file = GetIniDataFile()
    );

    /**
     * @brief Save the .ini settings of ImGui
     *
     * @remark Will automatically check and clear the "io.WantSaveIniSettings" flag
     *
     * @param ini Output
     * @return Value of io.WantSaveIniSettings
     */
    bool SaveImGuiSettingsData(ImGuiIniData& ini);
    /**
     * @brief Save the .ini settings of ImGui to disk
     *
     * @remark Will automatically check and clear the "io.WantSaveIniSettings" flag
     *
     * @param file Output
     * @return Value of io.WantSaveIniSettings
     */
    bool SaveImGuiSettingsDataToDisk(const std::filesystem::path& file);
    /**
     * @brief Load .ini settings data from disk
     *
     * @param file Input
     */
    void LoadImGuiSettingsDataFromDisk(const std::filesystem::path& file);
} // namespace ImGuiSR


#endif
