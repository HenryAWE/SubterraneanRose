/**
 * @file trace.hpp
 * @author HenryAWE
 * @brief Runtime debugging and stack tracing
 */

#ifndef SROSE_TRACE_trace_hpp_
#define SROSE_TRACE_trace_hpp_

#include <boost/stacktrace.hpp>


namespace srose::trace
{
    void OutputStackAndQuickExit(int sig) noexcept;

    /**
     * @brief Register handler for SIGILL, SIGSEGV and SIGABRT for this thread
     */
    void RegisterSignalHandler() noexcept;
} // namespace srose::trace


#endif
