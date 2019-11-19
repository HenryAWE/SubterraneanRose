/**
 * @file widget.cpp
 * @author HenryAWE
 * @brief Widget base
 */

#include "widget.hpp"
#include "../i18n/i18n.hpp"


namespace srose::ui
{
    Widget::Widget()
     : m_loc() {}

    std::locale Widget::imbue(const std::locale& loc)
    {
        return std::exchange(m_loc, loc);
    }

    std::string Widget::gettext(std::string_view id) const
    {
        auto& fct = std::use_facet<locale::TranslationFacet>(m_loc);
        return fct.get().gettext(id);
    }
} // namespace srose::ui
