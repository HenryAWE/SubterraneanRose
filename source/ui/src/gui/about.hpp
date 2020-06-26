/**
 * @file about.hpp
 * @author HenryAWE
 * @brief About page
 */

#ifndef SROSE_UI_GUI_about_hpp_
#define SROSE_UI_GUI_about_hpp_

#include <sr/ui/widget.hpp>
#include <sr/ui/node.hpp>


namespace srose::ui
{
    class About : public RootNode
    {
    public:
        typedef RootNode Base;

        About();

        void Update() override;

    private:
        void LoadI18nData() override;
    };
} // namespace srose::ui


#endif
