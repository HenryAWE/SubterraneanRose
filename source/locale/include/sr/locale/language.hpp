/**
 * @file language.hpp
 * @author HenryAWE
 * @brief Language
 */

#ifndef SROSE_LOCALE_language_hpp_
#define SROSE_LOCALE_language_hpp_

#include <utility>
#include <filesystem>
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
            m_iso(std::move(move.m_iso)) {}
        explicit Language(const std::filesystem::path& directory);

        [[nodiscard]]
        std::string gettext(std::string_view path);
        [[nodiscard]]
        std::string gettext(std::string_view path, std::string_view alternate);

        [[nodiscard]]
        const std::string& name() const noexcept { return m_name; }
        [[nodiscard]]
        const std::string& iso() const noexcept { return m_iso; }

    private:
        util::string_tree<std::string> m_tr;
        std::string m_name, m_iso;

        void LoadSpecStrings();
    };
} // namespace srose::locale


#endif
