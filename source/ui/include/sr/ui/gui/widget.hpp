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
#include <optional>
#include <map>
#include <boost/noncopyable.hpp>
#include <sr/util/string_comparator.hpp>
#include <sr/i18n/i18n.hpp>


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

        void SetWindowSubtitle() { OnSetWindowSubtitle(); }

    protected:
        virtual void OnImbue();
        virtual void OnSetWindowSubtitle();

        void DoSetWindowSubtitle(const char* subtitle = nullptr);

        void AddString(
            std::string id,
            std::optional<std::string> path,
            std::string_view prefix = std::string_view(),
            std::string_view suffix = std::string_view()
        );
        const std::string& GetString(std::string_view id);

    private:
        struct StringData
        {
            const std::optional<std::string> path;
            const std::string prefix;
            const std::string suffix;

            std::string data;

            StringData(const StringData&) = default;
            StringData(StringData&&) = default;
            StringData(
                const util::string_tree<std::string>& tree,
                std::optional<std::string> path_,
                std::string prefix_,
                std::string suffix_
            ) : path(std::move(path_)), prefix(std::move(prefix_)), suffix(std::move(suffix_))
            {
                if(path.has_value())
                    data = prefix + tree.get_value(*path) + suffix;
                else
                    data = prefix + suffix;
            }

            void Load(const util::string_tree<std::string>& tree);
        };
        std::map<std::string, StringData, util::string_comparator<char>> m_string_data;
    };
} // namespace srose::ui


#endif
