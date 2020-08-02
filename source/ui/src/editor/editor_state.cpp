/**
 * @file editor_state.cpp
 * @author HenryAWE
 * @brief Editor state machine
 */

#include "editor_state.hpp"
#include <sr/trace/log.hpp>


namespace srose::ui::editor
{
    StateWelcomeWindow::StateWelcomeWindow() = default;

    StateWelcomeWindow::~StateWelcomeWindow() = default;

    sc::result StateWelcomeWindow::react(const EventUpdate&)
    {
        auto& editor = outermost_context().editor;

        editor.GetWelcomeWindow()->Update();

        if(editor.GetWelcomeWindow()->visible())
            return discard_event();
        else
            return transit<StateIdle>();
    }

    StateIdle::StateIdle()
    {
        BOOST_LOG_TRIVIAL(info) << "[UI] Enter StateIdle";
    }

    StateIdle::~StateIdle()
    {
        BOOST_LOG_TRIVIAL(info) << "[UI] Leave StateIdle";
    }
} // namespace srose::ui::editor
