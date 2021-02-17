/**
 * @file imgui_ini.cpp
 * @author HenryAWE
 * @brief Handling imgui.ini
 */

#include <imgui_ini.hpp>
#include <fstream>
#include <sr/filesystem/common.hpp>


namespace ImGuiSR
{
    std::filesystem::path GetIniDataFile()
    {
        return srose::filesystem::GetAppData() / "imguisr.dat";
    }

    void SaveIniDataToDisk(
        const ImGuiIniData& ini,
        const std::filesystem::path file
    ) {
        std::ofstream ofs(file, std::ios_base::binary);
        ofs.write(ini.data(), ini.size() * sizeof(ImGuiIniData::value_type));
    }
    void LoadIniDataFromDisk(
        ImGuiIniData& ini,
        const std::filesystem::path file
    ) {
        std::ifstream ifs(file, std::ios_base::binary);
        if(!ifs.good())
        {
            ini.clear();
            return;
        }
        auto size = std::filesystem::file_size(file);
        ini.resize(size);
        ifs.read(ini.data(), ini.size() * sizeof(ImGuiIniData::value_type));
    }

    bool SaveImGuiSettingsData(ImGuiIniData& ini)
    {
        auto& io = ImGui::GetIO();

        if(io.IniFilename != nullptr || io.WantSaveIniSettings == false)
            return false;

        size_t size;
        const char* data = ImGui::SaveIniSettingsToMemory(&size);
        ini.resize(size);
        std::copy(data, data + size, ini.begin());

        io.WantSaveIniSettings = false;
        return true;
    }
    bool SaveImGuiSettingsDataToDisk(const std::filesystem::path& file)
    {
        auto& io = ImGui::GetIO();

        if(io.IniFilename != nullptr || io.WantSaveIniSettings == false)
            return false;

        ImGui::SaveIniSettingsToDisk(file.u8string().c_str());

        io.WantSaveIniSettings = false;
        return true;
    }
    void LoadImGuiSettingsDataFromDisk(const std::filesystem::path& file)
    {
        if(!std::filesystem::exists(file))
            return;

        ImGui::LoadIniSettingsFromDisk(file.u8string().c_str());
    }
} // namespace ImGuiSR
