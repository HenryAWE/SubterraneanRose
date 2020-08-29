/**
 * @file stralgo.hpp
 * @author HenryAWE
 * @brief String algorithm
 */

#ifndef SRTL_stralgo_hpp_
#define SRTL_stralgo_hpp_

#include <string>
#include <iterator>
#include <type_traits>


namespace srtl
{
    template <
        typename DelimT,
        typename CharT = char,
        typename Traits = std::char_traits<CharT>
    >
    class ostream_joiner
    {
    public:
        typedef CharT char_type;
        typedef Traits traits_type;
        typedef std::basic_ostream<CharT, Traits> ostream_type;
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;
        typedef std::output_iterator_tag iterator_category;

        ostream_joiner(ostream_type& os, const DelimT& delimiter)
            : m_first(true), m_os(&os), m_delimiter(delimiter) {}
        ostream_joiner(ostream_type& os, DelimT&& delimiter)
            : m_first(true), m_os(&os), m_delimiter(std::move(delimiter)) {}

        template <typename T>
        ostream_joiner& operator=(const T& value)
        {
            if(!m_first)
                *m_os << m_delimiter;
            m_first = false;
            *m_os << value;

            return *this;
        }

        ostream_joiner& operator*() noexcept { return *this; }

        ostream_joiner& operator++() noexcept { return *this; }
        ostream_joiner& operator++(int) noexcept { return *this; }

    private:
        bool m_first;
        ostream_type* m_os;
        DelimT m_delimiter;
    };

    template <typename DelimT, typename CharT, typename Traits>
    ostream_joiner<std::decay_t<DelimT>, CharT, Traits> make_ostream_joiner(
        std::basic_ostream<CharT, Traits>& os,
        DelimT&& delimiter
    ) {
        return ostream_joiner<std::decay_t<DelimT>, CharT, Traits>(
            os,
            std::forward<DelimT>(delimiter)
        );
    }
} // namespace srtl


#endif
