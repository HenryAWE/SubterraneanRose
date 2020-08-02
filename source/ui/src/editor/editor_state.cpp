/**
 * @file editor_state.cpp
 * @author HenryAWE
 * @brief Editor state machine
 */

#include "editor_state.hpp"
#include <sr/trace/log.hpp>


namespace srose::ui::editor
{
    StateIdle::StateIdle()
    {
        BOOST_LOG_TRIVIAL(info) << "[UI] Enter StateIdle";
    }

    StateIdle::~StateIdle()
    {
        BOOST_LOG_TRIVIAL(info) << "[UI] Leave StateIdle";
    }
} // namespace srose::ui::editor
