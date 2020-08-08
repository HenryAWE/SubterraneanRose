/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief Audio demo
 */

#ifndef SROSE_AUDIO_demo_hpp_
#define SROSE_AUDIO_demo_hpp_

#include <vector>
#include <sr/audio/aumgr.hpp>
#include <sr/ui/node.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::audio
{
    class AudioDemoWindow : public ui::StandaloneNode
    {
    public:
        typedef StandaloneNode Base;

        AudioDemoWindow();

        ~AudioDemoWindow();

        void Update() override;

    private:
        void UpdateMenuBar();
        void UpdateTabBar();

        bool m_show_music_demo = true;
        std::vector<char> m_music_location_cache;
        Music m_music;
        void MusicTabItem();

        bool m_show_information = false;
        std::vector<std::string> m_devices_list;
        void LoadDevicesList();
        void InformationTabItem();
    };
} // namespace srose::audio

#endif


#endif
