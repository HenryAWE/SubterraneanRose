/**
 * @file state.hpp
 * @author HenryAWE
 * @brief Editor state machine
 */

#ifndef SROSE_UI_EDITOR_state_hpp_
#define SROSE_UI_EDITOR_state_hpp_

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/termination.hpp>
#include "window.hpp"


namespace srose::ui::editor
{
    namespace sc = boost::statechart;

    class EventUpdate;
    class EventLoadProject;
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
            sc::custom_reaction<EventLoadProject>,
            sc::termination<EventQuit>
        > reactions;

        StateWelcomeWindow();

        ~StateWelcomeWindow();

        sc::result react(const EventUpdate&);
        sc::result react(const EventLoadProject& e);
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
    class EventLoadProject : public sc::event<EventLoadProject>
    {
    public:
        enum LoadFrom : int
        {
            LOAD_OPEN_FILE,
            LOAD_NEW_INSTANCE
        };

        const LoadFrom from;

        EventLoadProject(LoadFrom from_) : from(from_) {}
    };
    class EventCancel : public sc::event<EventCancel> {};
    class EventResetEditor : public sc::event<EventResetEditor> {};
    class EventQuit : public sc::event<EventQuit> {};
} // namespace srose::ui::editor


#endif
