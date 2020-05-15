/**
 * @file conwin.hpp
 * @author HenryAWE
 * @brief Graphical console
 */

#ifndef SROSE_UI_GUI_conwin_hpp_
#define SROSE_UI_GUI_conwin_hpp_

#include <tuple>
#include <deque>
#include <sr/ui/widget.hpp>
#include <SDL_log.h>


namespace srose::ui
{
    class ConsoleWindow : public Widget
    {
    public:
        ConsoleWindow();

        ~ConsoleWindow();

        bool open = true;
        void Update() override;

        bool verbose = false;

    private:
        void Load();

        void UpdateLogViewerTabItem();

        std::deque<std::tuple<SDL_LogPriority, std::string>> m_logbuf;
        bool m_logchanged = false;
    };
} // namespace srose::ui


#endif
