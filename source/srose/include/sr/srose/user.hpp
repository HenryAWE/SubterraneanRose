/**
 * @file user.hpp
 * @author HenryAWE
 * @brief User profile
 */

#ifndef SROSE_SROSE_user_hpp_
#define SROSE_SROSE_user_hpp_

#include <string>
#include <iosfwd>
#include "config.hpp"
#include <boost/property_tree/ptree.hpp>


namespace srose
{
    class UserProfile
    {
        boost::property_tree::ptree m_data;
        std::string m_name;
    public:
        explicit UserProfile(std::istream& is);
        explicit UserProfile(std::string name);
        UserProfile(const UserProfile&) = delete;

        void Create(std::string name);
        void Load(std::istream& is);
        void Save(std::ostream& os);

        std::string Get(const std::string& pt) const;
        void Set(const std::string& pt, std::string val);

        void name(std::string val) { m_name.swap(val); }
        [[nodiscard]]
        constexpr const std::string& name() const noexcept { return m_name; }

        [[nodiscard]]
        explicit operator const std::string&() const noexcept;
        [[nodiscard]]
        explicit operator std::string_view() const noexcept { return m_name; }

        struct Comparator
        {
            typedef void is_transparent;

            template <typename T, typename U>
            [[nodiscard]]
            bool operator()(const T& lhs, const U& rhs) const noexcept
            {
                return std::string_view(lhs) < std::string_view(rhs);
            }
        };
    };
} // namespace srose


#endif
