/**
 * @file configmenu.hpp
 * @author HenryAWE
 * @brief Configuration panel
 */

#ifndef SROSE_UI_GUI_configmenu_hpp_
#define SROSE_UI_GUI_configmenu_hpp_

#include <vector>
#include <utility>
#include "widget.hpp"


namespace srose::ui
{
    class ConfigPanel : public Widget
    {
    public:
        ConfigPanel();

        void Update() override;

    private:
        void LoadButtons();

        using callback_type = void(ConfigPanel::*)();
        std::vector<std::pair<std::string, callback_type>> m_buttons;

        void Button_Video();
        void Button_Language();
        void Button_Return();

        callback_type m_content_func = nullptr;

        void Content_Video();
    };
} // namespace srose::ui


#endif
