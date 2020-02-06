/**
 * @file editor_window.hpp
 * @author HenryAWE
 * @brief Editor window
 */

#ifndef SROSE_UI_GUI_editor_window_hpp_
#define SROSE_UI_GUI_editor_window_hpp_

#include <sr/ui/property.hpp>
#include <imguisr.h>
#include "../widget.hpp"


namespace srose::ui::editor
{
    class EditorWindow : public Widget
    {
    public:
        EditorWindow();

        void Update() override;

    private:
        std::string m_title;
        void LoadAll();

        void UpdateMenuBar();
    };
} // namespace srose::ui


#endif
