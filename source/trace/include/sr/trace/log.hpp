/**
 * @file log.hpp
 * @author HenryAWE
 * @brief Logging
 */

#ifndef SROSE_TRACE_log_hpp_
#define SROSE_TRACE_log_hpp_

#include <boost/log/trivial.hpp>


namespace srose::trace
{
    void InitializeLogger();
    void RedirectSDLOutput(bool redirect = true);
} // namespace srose::trace


#endif
