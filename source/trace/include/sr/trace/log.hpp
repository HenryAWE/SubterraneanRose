/**
 * @file log.hpp
 * @author HenryAWE
 * @brief Logging
 */

#ifndef SROSE_TRACE_log_hpp_
#define SROSE_TRACE_log_hpp_

#include <memory>
#include <boost/log/trivial.hpp>


namespace srose::trace
{
    void InitializeLogger();
    void AddStream(const std::shared_ptr<std::ostream>& os);
    void RedirectSDLOutput(bool redirect = true);
} // namespace srose::trace


#endif
