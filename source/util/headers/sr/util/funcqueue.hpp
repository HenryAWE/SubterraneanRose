/**
 * @file funcqueue.hpp
 * @author HenryAWE
 * @brief Function queue
 */

#ifndef SROSE_UTIL_HEADERS_funcqueue_hpp_
#define SROSE_UTIL_HEADERS_funcqueue_hpp_

#include <utility>
#include <deque>
#include <functional>
#include <cstdint>
#include <cassert>


namespace srose::util
{
    class funcqueue
    {
    public:
        typedef std::uint32_t int_t;
        typedef std::function<void()> func_t;
        typedef funcqueue self;

        self& then(func_t f)
        {
            m_q.push_back(std::make_pair(1, std::move(f)));
            return *this;
        }
        self& repeat(int_t times, func_t f)
        {
            if(times == 0)
                return *this;
            m_q.push_back(std::make_pair(times, std::move(f)));
            return *this;
        }

        void invoke()
        {
            if(m_q.empty())
                return;

            auto& func = m_q.front();
            assert(func.first != 0);
            try
            {
                func.second();
            }
            catch(...)
            {
                if(--func.first == 0)
                    m_q.pop_front();
                throw;
            }
            if(--func.first == 0)
                m_q.pop_front();
        }

    private:
        std::deque<std::pair<int_t, func_t>> m_q;
    };
} // namespace srose::util


#endif
