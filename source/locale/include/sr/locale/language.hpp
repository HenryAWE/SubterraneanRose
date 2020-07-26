/**
 * @file language.hpp
 * @author HenryAWE
 * @brief Language
 */

#ifndef SROSE_LOCALE_language_hpp_
#define SROSE_LOCALE_language_hpp_

#include <utility>
#include <memory>
#include <iostream>
#include <set>
#include <sr/core/macros.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/string_tree.hpp>
#include <sr/util/semver.hpp>


namespace srose::locale
{
    enum TextErrorAction : std::uint32_t
    {
        SRLC_RETURN_EMPTY_STRING = 0,
        SRLC_RETURN_REQUEST = 1,
        SRLC_RETURN_ERROR_STRING = 2,
        SRLC_USE_FALLBACK = 3,
        SRLC_THROW_EXCEPTION = 4
    };

    class Language : std::enable_shared_from_this<Language>
    {
    public:
        struct Comparator
        {
            typedef void is_transparent;

            template <typename T, typename U>
            bool operator()(T&& lhs, U&& rhs) const noexcept
            {
                return View(lhs) < View(rhs);
            }

        private:
            [[nodiscard]]
            static constexpr std::string_view View(std::string_view value) noexcept { return value; }
            [[nodiscard]]
            static std::string_view View(const Language& value) noexcept { return value.GetId(); }
            static std::string_view View(const std::shared_ptr<Language>& value)
            {
                return value ? value->GetId() : std::string_view();
            }
        };

        Language();
        Language(const Language& other);
        Language(Language&& move) noexcept;
        explicit Language(const filesystem::path& file);
        explicit Language(std::istream& is);

        [[nodiscard]]
        std::string gettext(std::string_view path);
        struct use_fallback_t {};
        static constexpr use_fallback_t use_fallback{};
        [[nodiscard]]
        std::string gettext(std::string_view path, use_fallback_t);
        [[nodiscard]]
        std::string gettext(std::string_view path, std::string_view alternate);

        [[nodiscard]]
        const std::string& GetId() const noexcept { return m_id; }
        [[nodiscard]]
        const std::string& GetName() const noexcept { return m_name; }
        [[nodiscard]]
        const util::SemVer& GetVersion() const noexcept { return m_version; }
        [[nodiscard]]
        constexpr const util::string_tree<std::string>& GetTextStringTree() const noexcept { return m_text; }
        [[nodiscard]]
        const std::optional<std::string>& default_str() const noexcept { return m_default; }

        void fallback(std::shared_ptr<Language> value) noexcept { m_fallback.swap(value); }
        [[nodiscard]]
        const std::shared_ptr<Language>& fallback() const noexcept { return m_fallback; }

    private:
        std::string m_id;
        std::string m_name;
        util::SemVer m_version;
        util::string_tree<std::string> m_text;
        std::optional<std::string> m_default;

        std::shared_ptr<Language> m_fallback;

        void Decode(std::istream& is);
        void DecodeTextBlock(std::istream& is);

        void LoadSpecStrings();
    };

    using LanguageSet = std::set<
        std::shared_ptr<Language>,
        Language::Comparator
    >;

    std::locale SRSCALL CreateTranslation(const std::locale& in, std::shared_ptr<Language> lang);

    class TranslationFacet : public std::locale::facet
    {
        std::shared_ptr<Language> m_lang;
    public:
        static std::locale::id id;

        TranslationFacet(const TranslationFacet&) = delete;
        TranslationFacet(std::shared_ptr<Language> lang) noexcept
            : m_lang(std::move(lang)) {}

        [[nodiscard]]
        Language& get() const noexcept { return *m_lang.get(); }
        [[nodiscard]]
        const std::shared_ptr<Language>& getptr() const noexcept { return m_lang; }
    };

    class TranslationHelper
    {
    public:
        std::string_view path;

        TranslationHelper(std::string_view path_) noexcept
            : path(path_) {}

        friend std::ostream& operator<<(std::ostream& os, const TranslationHelper& helper)
        {
            auto lc = os.getloc();
            auto& tr = std::use_facet<TranslationFacet>(lc);
            os << tr.get().gettext(helper.path);

            return os;
        }

        std::string str() const;

        operator std::string() const { return str(); }
    };
} // namespace srose::locale

#define SRTR(path) ((::srose::locale::TranslationHelper)(path))


#endif
