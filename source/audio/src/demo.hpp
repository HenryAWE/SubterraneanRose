/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief Audio demo
 */

#ifndef SROSE_AUDIO_demo_hpp_
#define SROSE_AUDIO_demo_hpp_

#include <vector>
#include <sr/ui/gui/widget.hpp>


namespace srose::audio
{
    class AudioDemoWindow : public ui::Widget
    {
    public:
        AudioDemoWindow();

        ~AudioDemoWindow();

        bool open = true;
        void Update() override;

    private:
        void UpdateMenuBar();
        void UpdateTabBar();

        bool m_show_information = false;
        std::vector<std::string> m_devices_list;
        void LoadDevicesList();
        void InformationTabItem();
    };
} // namespace srose::audio


#endif
