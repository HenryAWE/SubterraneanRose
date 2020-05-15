/**
 * @file log.cpp
 * @author HenryAWE
 * @brief Logging
 */

#include <sr/trace/log.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>


namespace srose::trace
{
    void InitializeLogger()
    {
        using namespace boost;
        namespace kw = log::keywords;

        std::string fmt = "[%TimeStamp%][%Severity%]: %Message%";

        log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
        
        log::add_console_log(
            std::cout,
            kw::format = fmt,
            kw::auto_flush = true
        );

        log::add_file_log(
            kw::file_name = "log/%Y-%m-%d.log",
            kw::open_mode = std::ios_base::out | std::ios_base::app,
            kw::rotation_size = 1 * 1024 *1024, // 1MB
            kw::max_size = 20 * 1024 * 1024, // 20MB
            kw::format = fmt,
            kw::auto_flush = true,
            kw::filter = log::trivial::severity >= log::trivial::warning
        );

        log::add_common_attributes();
    }
} // namespace srose::trace
