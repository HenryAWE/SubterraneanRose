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
        using callback_type = void(ConfigPanel::*)();
        std::vector<std::pair<std::string, callback_type>> m_buttons;

        void Button_Return();
    };
} // namespace srose::ui


#endif
