/**
 * @file language.hpp
 * @author HenryAWE
 * @brief Language
 */

#ifndef SROSE_LOCALE_language_hpp_
#define SROSE_LOCALE_language_hpp_

#include <utility>
#include <iostream>
#include <sr/core/macros.h>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/string_tree.hpp>


namespace srose::locale
{
    class Language
    {
    public:
        Language();
        Language(Language&& move) noexcept
            : m_tr(std::move(move.m_tr)),
            m_name(std::move(move.m_name)),
            m_iso(std::move(move.m_iso)),
            m_default(std::move(move.m_default)) {}
        explicit Language(const std::filesystem::path& directory);

        [[nodiscard]]
        std::string gettext(std::string_view path);
        [[nodiscard]]
        std::string gettext(std::string_view path, std::string_view alternate);

        [[nodiscard]]
        const std::string& name() const noexcept { return m_name; }
        [[nodiscard]]
        const std::string& iso() const noexcept { return m_iso; }
        [[nodiscard]]
        const std::optional<std::string>& default_str() const noexcept { return m_default; }

    private:
        util::string_tree<std::string> m_tr;
        std::string m_name, m_iso;
        std::optional<std::string> m_default;

        void LoadSpecStrings();
    };

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
