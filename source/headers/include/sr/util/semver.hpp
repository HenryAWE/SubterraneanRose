/**
 * @file semver.hpp
 * @author HenryAWE
 * @brief Semantic version library of the Subterranean Rose Project
 */

#ifndef SROSE_UTIL_HEADERS_semver_hpp_
#define SROSE_UTIL_HEADERS_semver_hpp_

#include <string>
#ifdef __GNUC__
#   undef major
#   undef minor
#endif


namespace srose::util
{
    class SemVer
    {
    public:
        typedef unsigned int value_type;

        SemVer() noexcept = default;
        SemVer(const SemVer&) noexcept = default;
        SemVer(value_type major, value_type minor, value_type patch) noexcept
            : m_major(major), m_minor(minor), m_patch(patch) {}

        SemVer& operator=(const SemVer& rhs) noexcept = default;

        [[nodiscard]]
        constexpr value_type& major() noexcept { return m_major; }
        [[nodiscard]]
        constexpr const value_type& major() const noexcept { return m_major; }
        [[nodiscard]]
        constexpr value_type& minor() noexcept { return m_minor; }
        [[nodiscard]]
        constexpr const value_type& minor() const noexcept { return m_minor; }
        [[nodiscard]]
        constexpr value_type& patch() noexcept { return m_patch; }
        [[nodiscard]]
        constexpr const value_type& patch() const noexcept { return m_patch; }

        int Compare(const SemVer& other) const noexcept
        {
            auto result = Cmp(m_major, other.m_major);
            if(result == 0) result = Cmp(m_minor, other.m_minor);
            if(result == 0) result = Cmp(m_patch, other.m_patch);
            return result;
        }

        bool operator==(const SemVer& rhs) const noexcept { return Compare(rhs) == 0; }
        bool operator!=(const SemVer& rhs) const noexcept { return Compare(rhs) != 0; }
        bool operator<(const SemVer& rhs) const noexcept { return Compare(rhs) < 0; }
        bool operator<=(const SemVer& rhs) const noexcept { return Compare(rhs) <= 0; }
        bool operator>(const SemVer& rhs) const noexcept { return Compare(rhs) > 0; }
        bool operator>=(const SemVer& rhs) const noexcept { return Compare(rhs) >= 0; }

    private:
        value_type m_major = 0, m_minor = 0, m_patch = 0;

        // After upgrading to C++20, this function will be replaced by the spaceship operator (<=>)
        static int Cmp(value_type lhs, value_type rhs) noexcept
        {
            if(lhs == rhs) return 0;
            else if(lhs < rhs) return -1;
            else /*if (lhs > rhs)*/ return 1;
        }
    };
} // namespace srose::util


#endif
