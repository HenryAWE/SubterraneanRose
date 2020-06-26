/**
 * @file widget.hpp
 * @author HenryAWE
 * @brief Widget base
 */

#ifndef SROSE_UI_widget_hpp_
#define SROSE_UI_widget_hpp_

#include <utility>
#include <memory>
#include <locale>
#include <optional>
#include <map>
#include <boost/noncopyable.hpp>
#include <sr/util/string_comparator.hpp>
#include <sr/i18n/i18n.hpp>


namespace srose::ui
{
    class Widget : public std::enable_shared_from_this<Widget>, public boost::noncopyable
    {
    public:
        Widget();

        virtual ~Widget();
    };
} // namespace srose::ui


#endif
