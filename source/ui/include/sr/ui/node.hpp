/**
 * @file node.hpp
 * @author HenryAWE
 * @brief UI element node
 */

#ifndef SROSE_UI_node_hpp_
#define SROSE_UI_node_hpp_

#include <memory>
#include <vector>
#include <tuple>
#include <variant>
#include <boost/signals2.hpp>
#include <sr/util/string_comparator.hpp>
#include <sr/locale/language.hpp>


namespace srose::ui
{
    class BaseNode : public std::enable_shared_from_this<BaseNode>
    {
    protected:
        BaseNode();

    public:
        typedef void Base;

        virtual ~BaseNode();

        virtual void Update();
    };


    class I18nNode : public BaseNode
    {
        std::shared_ptr<locale::Language> m_lang;
        boost::signals2::connection m_connection;

    public:
        typedef BaseNode Base;

        I18nNode();
        I18nNode(const std::locale& loc);
        I18nNode(std::shared_ptr<locale::Language> lang);

        ~I18nNode();

        void Update() override;

        void SetLanguage(const std::locale& loc);
        void SetLanguage(std::shared_ptr<locale::Language> lang);

        void Connect(boost::signals2::signal<void(const std::locale&)>& notifier);

        std::string gettext(std::string_view id);

        const std::shared_ptr<locale::Language>& getptr() const noexcept { return m_lang; }

    protected:
        virtual void LoadI18nData();

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

    class RootNode : public I18nNode
    {
        std::string m_id;
        int m_flags;
    public:
        typedef I18nNode Base;

        RootNode(std::string_view id);
        RootNode(std::string_view id, std::shared_ptr<locale::Language> lang);

        void Update() override;

    protected:
        void SetFlags(int flags) { m_flags = flags; }

        struct ContextGuard
        {
            RootNode& node;
            bool value;

            ContextGuard(RootNode& node_, bool value_) noexcept
                : node(node_), value(value_) {}
            ~ContextGuard() noexcept { node.EndContext(); }

            [[nodiscard]]
            constexpr operator bool() const noexcept { return value; }
        };

        ContextGuard BeginContext();

    private:
        void EndContext() noexcept;
    };

    class StandaloneNode : public I18nNode
    {
        bool m_open = true;
        std::string m_title;
        std::string m_name;
        std::string m_id;
    public:
        typedef I18nNode Base;

        StandaloneNode(std::string title, std::string name = std::string());
        StandaloneNode(std::string title, std::string name, std::shared_ptr<locale::Language> lang);

        void Update() override;

        [[nodiscard]]
        constexpr bool visible() const noexcept { return m_open; }

        void Open() noexcept { m_open = true; }
        void Close() noexcept { m_open = false; }

        void SetId(std::string title);
        void SetId(std::string title, std::string name = std::string());

    protected:
        struct ContextGuard
        {
            StandaloneNode& node;
            bool value;

            ContextGuard(StandaloneNode& node_, bool value_) noexcept
                : node(node_), value(value_) {}
            ~ContextGuard() noexcept;

            [[nodiscard]]
            constexpr operator bool() const noexcept { return value; }
        };

        ContextGuard BeginContext();

    private:
        void EndContext() noexcept;
    };
} // namespace srose::ui


#endif
