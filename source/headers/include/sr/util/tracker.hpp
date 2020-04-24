/**
 * @file tracker.hpp
 * @author HenryAWE
 * @brief Variable tracker
 */

#ifndef SROSE_UTIL_HEADERS_tracker_hpp_
#define SROSE_UTIL_HEADERS_tracker_hpp_

#include <utility>


namespace srose::util
{
    using std::in_place_t;
    using std::in_place;

    struct init_as_clean_t{};
    static constexpr init_as_clean_t init_as_clean{};

    template <typename T>
    class tracked
    {
        bool m_clean = false;
        T m_storage{};
    public:
        tracked() = default;
        tracked(const tracked& rhs)
            : m_clean(false), m_storage(rhs.m_storage) {}
        tracked(tracked&& move)
            : m_clean(std::exchange(move.m_clean, false)),
            m_storage(std::move(move.m_storage)) {}
        template <typename... Args>
        explicit tracked(in_place_t, Args&&... args)
            : m_clean(false),
            m_storage(std::forward<Args>(args)...) {}
        template <typename... Args>
        explicit tracked(init_as_clean_t, Args&&... args)
            : m_clean(true),
            m_storage(std::forward<Args>(args)...) {}

        void assign(const T& var) { m_storage = var; mark_as_dirty(); }
        void assign(T&& var) { m_storage = std::move(var); mark_as_dirty(); }
        const T& value() const noexcept { return m_storage; }

        constexpr bool clean() const noexcept { return m_clean; }
        constexpr bool dirty() const noexcept { return !m_clean; }

        constexpr void mark_as_clean() noexcept { m_clean = true; }
        constexpr void mark_as_dirty() noexcept { m_clean = false; }
    };
} // namespace srose::util


#endif
