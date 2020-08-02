/**
 * @file welcome.hpp
 * @author HenryAWE
 * @brief Welcome window
 */

#ifndef SROSE_UI_MENU_welcome_hpp_
#define SROSE_UI_MENU_welcome_hpp_

#include <sr/ui/node.hpp>


namespace srose::ui::editor
{
    class WelcomeWindow : public StandaloneNode
    {
    public:
        typedef StandaloneNode Base;

        WelcomeWindow();

        ~WelcomeWindow();

        void Update() override;

    private:
        void LoadI18nData() override;
    };
} // namespace srose::ui::editor


#endif
