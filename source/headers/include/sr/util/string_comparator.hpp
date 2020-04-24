/**
 * @file string_comparator.hpp
 * @author HenryAWE
 * @brief String comparator
 */

#ifndef SROSE_UTIL_HEADERS_string_comparator_hpp_
#define SROSE_UTIL_HEADERS_string_comparator_hpp_

#include <string>
#include <string_view>


namespace srose::util
{
    template< 
        typename CharT,
        typename Traits = std::char_traits<CharT>,
        typename Allocator = std::allocator<CharT>
    >
    class string_comparator
    {
    public:
        typedef std::basic_string<CharT, Traits, Allocator> string_type;
        typedef std::basic_string_view<CharT, Traits> string_view_type;

        // Enable comparing across different type without constructing new instance
        typedef void is_transparent;

        bool operator()(string_view_type lhs, string_view_type rhs) const noexcept
        {
            return lhs < rhs;
        }
    };
} // namespace srose::util


#endif
