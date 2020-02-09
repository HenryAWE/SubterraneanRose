/**
 * @file widget.cpp
 * @author HenryAWE
 * @brief Widget base
 */

#include <sr/ui/gui/widget.hpp>
#include "../i18n/i18n.hpp"


namespace srose::ui
{
    Widget::Widget()
     : m_loc() {}

    std::locale Widget::imbue(const std::locale& loc)
    {
        auto lc = std::exchange(m_loc, loc);
        OnImbue();
        return std::move(lc);
    }

    std::string Widget::gettext(std::string_view id) const
    {
        auto& fct = std::use_facet<locale::TranslationFacet>(m_loc);
        return fct.get().gettext(id);
    }
} // namespace srose::ui
