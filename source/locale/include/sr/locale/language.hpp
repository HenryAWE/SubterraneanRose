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
    class language
    {
    public:
        language(language&& move) noexcept
            : m_tr(std::move(move.m_tr)) {}
        language(const std::filesystem::path& directory);

        std::string gettext(std::string_view path);
        std::string gettext(std::string_view path, std::string_view alternate);

    private:
        util::string_tree<std::string> m_tr;
    };
} // namespace srose::locale


#endif
