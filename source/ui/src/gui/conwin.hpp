/**
 * @file conwin.hpp
 * @author HenryAWE
 * @brief Graphical console
 */

#ifndef SROSE_UI_GUI_conwin_hpp_
#define SROSE_UI_GUI_conwin_hpp_

#include <tuple>
#include <deque>
#include <memory>
#include <sstream>
#include <sr/ui/node.hpp>


namespace srose::ui
{
    class ConsoleWindow : public StandaloneNode
    {
    public:
        typedef StandaloneNode Base;

        ConsoleWindow(std::string name = "conwin");

        ~ConsoleWindow();

        bool open = true;
        void Update() override;

        bool verbose = false;

    private:
        void LoadI18nData() override;

        void UpdateLogViewerTabItem();

        std::shared_ptr<std::stringstream> m_backend;

        std::deque<std::tuple<std::string>> m_logbuf;
        bool m_logchanged = false;
    };
} // namespace srose::ui


#endif
