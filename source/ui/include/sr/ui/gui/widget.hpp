/**
 * @file widget.hpp
 * @author HenryAWE
 * @brief Widget base
 */

#ifndef SROSE_UI_GUI_widget_hpp_
#define SROSE_UI_GUI_widget_hpp_

#include <utility>
#include <memory>
#include <locale>
#include <boost/noncopyable.hpp>


namespace srose::ui
{
    class Widget : public std::enable_shared_from_this<Widget>, public boost::noncopyable
    {
        std::locale m_loc;
    public:
        Widget();

        virtual ~Widget() = default;

        virtual void Update() {}

        [[nodiscard]]
        constexpr const std::locale& getloc() const noexcept { return m_loc; }
        std::locale imbue(const std::locale& loc);

        [[nodiscard]]
        std::string gettext(std::string_view id) const;

    protected:
        virtual void OnImbue() {}
    };
} // namespace srose::ui


#endif
