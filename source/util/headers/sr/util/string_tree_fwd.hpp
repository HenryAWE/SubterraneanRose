/**
 * @file string_tree_fwd.hpp
 * @author HenryAWE
 * @brief Forward declaration of the StringTree
 */

#ifndef SROSE_UTIL_HEADERS_string_tree_fwd_hpp_
#define SROSE_UTIL_HEADERS_string_tree_fwd_hpp_

#include <string>
#include <string_view>


namespace srose::util
{
    template <typename T,
         typename CharT,
         CharT Separator
    >
    class basic_string_tree;

    template <typename T, char Separator = '.'>
    using string_tree = basic_string_tree<T, char, Separator>;
} // namespace srose::util


#endif
