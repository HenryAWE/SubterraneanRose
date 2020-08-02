/**
 * @file editor_state.cpp
 * @author HenryAWE
 * @brief Editor state machine
 */

#ifndef SROSE_UI_MENU_editor_state_hpp_
#define SROSE_UI_MENU_editor_state_hpp_

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/termination.hpp>
#include "editor_window.hpp"


namespace srose::ui::editor
{
    namespace sc = boost::statechart;

    class EventUpdate;
    class EventCancel;
    class EventResetEditor;
    class EventQuit;

    class StateWelcomeWindow;
    class StateIdle;

    class EditorState : public sc::state_machine<EditorState, StateWelcomeWindow>
    {
    public:
        EditorState(EditorWindow& editor_) : editor(editor_) {}

        EditorWindow& editor;
    };

    class StateWelcomeWindow : public sc::simple_state<StateWelcomeWindow, EditorState>
    {
    public:
        typedef boost::mpl::list<
            sc::transition<EventCancel, StateIdle>,
            sc::custom_reaction<EventUpdate>,
            sc::termination<EventQuit>
        > reactions;

        StateWelcomeWindow();

        ~StateWelcomeWindow();

        sc::result react(const EventUpdate&);
    };

    class StateIdle : public sc::simple_state<StateIdle, EditorState>
    {
    public:
        typedef boost::mpl::list<
            sc::transition<EventResetEditor, StateWelcomeWindow>,
            sc::termination<EventQuit>
        > reactions;

        StateIdle();

        ~StateIdle();
    };

    class EventUpdate : public sc::event<EventUpdate> {};
    class EventCancel : public sc::event<EventCancel> {};
    class EventResetEditor : public sc::event<EventResetEditor> {};
    class EventQuit : public sc::event<EventQuit> {};
} // namespace srose::ui::editor


#endif
