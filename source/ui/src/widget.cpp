/**
 * @file widget.cpp
 * @author HenryAWE
 * @brief Widget base
 */

#include <sr/ui/widget.hpp>
#include <sr/i18n/i18n.hpp>


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

    void Widget::OnImbue()
    {
        auto& tree = std::use_facet<locale::TranslationFacet>(m_loc).get().GetStringTree();
        for(auto& i : m_string_data)
            i.second.Load(tree);
    }

    void Widget::StringData::Load(const util::string_tree<std::string>& tree)
    {
        if(path.has_value())
            data = prefix + tree.get_value(*path) + suffix;
    }

    void Widget::AddString(
        std::string id,
        std::optional<std::string> path,
        std::string_view prefix,
        std::string_view suffix
    ) {
        auto& tree = std::use_facet<locale::TranslationFacet>(m_loc).get().GetStringTree();
        StringData data(tree, std::move(path), std::string(prefix), std::string(prefix));
        m_string_data.emplace(std::make_pair(id, std::move(data)));
    }
    const std::string& Widget::GetString(std::string_view id)
    {
        auto iter = m_string_data.find(id);
        if(iter == m_string_data.end())
            throw std::out_of_range("[UI] Unknown string id: " + std::string(id));
        return iter->second.data;
    }
} // namespace srose::ui
