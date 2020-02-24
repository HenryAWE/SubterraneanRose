/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief Audio demo
 */

#ifndef SROSE_AUDIO_demo_hpp_
#define SROSE_AUDIO_demo_hpp_

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
    };
} // namespace srose::audio


#endif
