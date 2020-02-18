/**
 * @file configmenu.hpp
 * @author HenryAWE
 * @brief Configuration panel
 */

#ifndef SROSE_UI_GUI_configmenu_hpp_
#define SROSE_UI_GUI_configmenu_hpp_

#include <vector>
#include <utility>
#include <sr/ui/gui/widget.hpp>


namespace srose::ui
{
    class ConfigPanel : public Widget
    {
    public:
        ConfigPanel();

        void Update() override;

    private:
        void LoadButtons();
        void ResetStates();

        using callback_type = void(ConfigPanel::*)();
        std::vector<std::pair<std::string, callback_type>> m_buttons;

        void Button_Video();
        void Button_Language();
        void Button_Developer();
        void Button_Return();

        callback_type m_content_func = nullptr;

        std::string m_str_windowed;
        void Content_Video();
        std::string m_str_show_demo;
        bool m_show_demo = false;
        void Content_Developer();
        void Content_Language();
    };
} // namespace srose::ui


#endif
