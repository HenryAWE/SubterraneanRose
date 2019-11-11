/**
 * @file property.hpp
 * @author HenryAWE
 * @brief UI properties
 */

#ifndef SROSE_UI_property_hpp_
#define SROSE_UI_property_hpp_

#include <string>
#include <limits>
#include <algorithm>


namespace srose::ui
{
    template <typename T>
    class Property
    {
    public:
        typedef T value_type;

    private:
        std::string m_name;
        value_type m_value = 0, m_min = 0, m_max = 0;

    public:
        Property(std::string name,
            value_type init = value_type(),
            value_type min_ = std::numeric_limits<value_type>::min(), value_type max_ = std::numeric_limits<value_type>::max())
            : m_name(std::move(name)), m_value(init), m_min(min_), m_max(max_) {}

        [[nodiscard]]
        constexpr value_type min() const noexcept { return m_min; }
        void min(value_type var) noexcept { m_min = var; }
        [[nodiscard]]
        constexpr value_type max() const noexcept { return m_max; }
        void max(value_type var) noexcept { m_max = var; }
        [[nodiscard]]
        constexpr value_type value() const noexcept { return m_value; }
        void value(value_type var) noexcept { std::clamp(var, m_min, m_max); }
    };
} // namespace srose::ui


#endif