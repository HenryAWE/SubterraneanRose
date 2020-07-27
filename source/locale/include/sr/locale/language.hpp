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
    // Forward declaration
    class Language;

    enum TextErrorAction : std::uint32_t
    {
        SRLC_RETURN_EMPTY_STRING = 0,
        SRLC_RETURN_REQUEST = 1,
        SRLC_RETURN_ERROR_STRING = 2,
        SRLC_USE_FALLBACK = 3,
        SRLC_THROW_EXCEPTION = 4
    };

    struct LanguageComparator;

    using LanguageSet = std::set<
        std::shared_ptr<Language>,
        LanguageComparator
    >;

    class Language : std::enable_shared_from_this<Language>
    {
    public:
        Language();
        Language(const Language& other);
        Language(Language&& move) noexcept;
        explicit Language(const filesystem::path& file);
        explicit Language(std::istream& is);

        std::string GetText(std::string_view path);
        std::string GetTextWith(std::string_view path, TextErrorAction action);
        std::string GetTextOr(std::string_view path, std::string_view alternative);
        std::optional<std::string> GetTextOptional(std::string_view path);

        [[nodiscard]]
        const std::string& GetId() const noexcept { return m_id; }
        [[nodiscard]]
        const std::string& GetName() const noexcept { return m_name; }
        [[nodiscard]]
        const util::SemVer& GetVersion() const noexcept { return m_version; }
        [[nodiscard]]
        constexpr const util::string_tree<std::string>& GetTextStringTree() const noexcept { return m_text; }

        [[nodiscard]]
        TextErrorAction GetTextErrorAction() const noexcept { return m_text_error; }
        [[nodiscard]]
        const std::optional<std::string>& GetErrorString() const noexcept { return m_error_string; }
        [[nodiscard]]
        const std::optional<std::string>& GetFallbackId() const noexcept { return m_fallback_id; }
        [[nodiscard]]
        const std::shared_ptr<Language>& GetFallback() const noexcept { return m_fallback; }

        void LinkFallback(LanguageSet& langs);

    private:
        std::string m_id;
        std::string m_name;
        util::SemVer m_version;
        util::string_tree<std::string> m_text;

        TextErrorAction m_text_error = SRLC_RETURN_EMPTY_STRING;
        std::optional<std::string> m_error_string;
        std::optional<std::string> m_fallback_id;
        std::shared_ptr<Language> m_fallback;

        void Decode(std::istream& is);
        void DecodeTextBlock(std::istream& is);
    };

    struct LanguageComparator
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

    std::shared_ptr<Language> SearchClosest(LanguageSet& langs, const std::string& id);

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
            os << tr.get().GetText(helper.path);

            return os;
        }

        std::string str() const;

        operator std::string() const { return str(); }
    };
} // namespace srose::locale

#define SRTR(path) ((::srose::locale::TranslationHelper)(path))


#endif
