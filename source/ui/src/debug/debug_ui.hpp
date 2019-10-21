/**
 * @file debug_ui.hpp
 * @author HenryAWE
 * @brief UI for debugging
 */

#ifndef SROSE_UI_debug_ui_hpp_
#define SROSE_UI_debug_ui_hpp_


namespace srose::ui
{
    struct DebugUI
    {
        bool draw_overlay = false;

        DebugUI(bool* p_open = nullptr);

        void Update();

    private:
        bool* m_p_open = nullptr;

        void DrawOverlay();
    };
} // namespace srose::ui


#endif
