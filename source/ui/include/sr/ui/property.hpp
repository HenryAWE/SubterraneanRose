/**
 * @file property.hpp
 * @author HenryAWE
 * @brief UI properties
 */

#ifndef SROSE_UI_property_hpp_
#define SROSE_UI_property_hpp_

#include <string>
#undef min
#undef max
#include <limits>
#include <algorithm>
#include <type_traits>


namespace srose::ui
{
    template <typename T>
    class Property
    {
        static_assert(std::is_arithmetic_v<T>, "");
    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;

    private:
        std::string m_name;
        value_type m_value = 0, m_min = 0, m_max = 0;

    public:
        Property(std::string name,
            value_type init = value_type(),
            value_type min_ = std::numeric_limits<value_type>::min(), value_type max_ = std::numeric_limits<value_type>::max())
            : m_name(std::move(name)),
            m_value(std::move(init)), m_min(std::move(min_)), m_max(std::move(max_)) {}

        [[nodiscard]]
        constexpr const std::string& name() const noexcept { return m_name; }

        [[nodiscard]]
        constexpr value_type min() const noexcept { return m_min; }
        constexpr void min(value_type var) noexcept { m_min = var; }
        [[nodiscard]]
        constexpr value_type max() const noexcept { return m_max; }
        constexpr void max(value_type var) noexcept { m_max = var; }
        [[nodiscard]]
        constexpr value_type value() const noexcept { return m_value; }
        void value(const_reference var) noexcept { m_value = std::clamp(var, m_min, m_max); }
    };

    template <>
    class Property<std::string>
    {
    public:
        typedef std::string value_type;

    private:
        std::string m_name;
        std::string m_value;

    public:
        Property(std::string name, value_type init = "")
            : m_name(std::move(name)), m_value(std::move(init)) {}

        [[nodiscard]]
        constexpr const std::string& name() const noexcept { return m_name; }

        [[nodiscard]]
        constexpr const value_type& value() const noexcept { return m_value; }
        void value(value_type var) noexcept { m_value = std::move(var); }
    };
} // namespace srose::ui


#endif
