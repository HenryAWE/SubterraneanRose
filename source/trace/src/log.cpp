/**
 * @file log.cpp
 * @author HenryAWE
 * @brief Logging
 */

#include <sr/trace/log.hpp>
#include <iomanip> // std::put_time
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <SDL.h>
#include <sr/filesystem/common.hpp>


namespace srose::trace
{
    constexpr char SR_LOG_FMT[] = "[%TimeStamp%][%Severity%]: %Message%";

    static void ColoredFormatter(const boost::log::record_view& rec, boost::log::formatting_ostream& os)
    {
        using namespace boost;

        auto severity = rec[log::trivial::severity];
        assert(severity);

        // Set the color
        switch(severity.get())
        {
        default:
        case log::trivial::severity_level::trace:
        case log::trivial::severity_level::debug:
            os << "\033[1m";
            break;
        case log::trivial::severity_level::info:
            os << "\033[1;32m";
            break;
        case log::trivial::severity_level::warning:
            os << "\033[1;33m";
            break;
        case log::trivial::severity_level::error:
            os << "\033[1;31m";
            break;
        case log::trivial::severity_level::fatal:
            os << "\033[1;35m";
            break;
        }

        auto timestamp = log::extract<posix_time::ptime>("TimeStamp", rec);
        if(timestamp)
        {
            std::tm ts = posix_time::to_tm(*timestamp);
            os << '[' << std::put_time(&ts, "%H:%M:%S") << ']';
        }
        else
        {
            os << "[]";
        }
        os << '[' << severity << ']';
        os << ": ";

        if(severity)
        {
            // Restore the color
            os << "\033[0m";
        }

        os << rec[log::expressions::smessage];
    }

    void InitializeLogger()
    {
        using namespace boost;
        namespace kw = log::keywords;

        std::string fmt = SR_LOG_FMT;

        log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
        
        auto console = log::add_console_log(
            std::cout,
            kw::auto_flush = true
        );
        console->set_formatter(&ColoredFormatter);

        // Sometimes application doesn't have the write permission of the installation directory
        // Program will crash if it cannot write log data into that directory
        auto log_file_folder = filesystem::GetLogFolder().string();
        log::add_file_log(
            kw::file_name = log_file_folder + "/%Y-%m-%d.log",
            kw::open_mode = std::ios_base::out | std::ios_base::app,
            kw::rotation_size = 1 * 1024 *1024, // 1MB
            kw::max_size = 20 * 1024 * 1024, // 20MB
            kw::format = fmt,
            kw::auto_flush = true,
            kw::filter = log::trivial::severity >= log::trivial::warning
        );

        log::add_common_attributes();
    }

    void AddStream(const std::shared_ptr<std::ostream>& os)
    {
        boost::shared_ptr<std::ostream> ptr(
            os.get(),
            [p = os](...) mutable { p.reset(); }
        );

        using namespace boost;
        namespace kw = log::keywords;

        typedef log::sinks::synchronous_sink<log::sinks::text_ostream_backend> text_sink;
        boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

        sink->locked_backend()->add_stream(std::move(ptr));

        log::core::get()->add_sink(std::move(sink));
    }

    const char* to_chars(int category)
    {
        switch(category)
        {
        case SDL_LOG_CATEGORY_APPLICATION:
            return "Application";
        case SDL_LOG_CATEGORY_ERROR:
            return "Error";
        case SDL_LOG_CATEGORY_ASSERT:
            return "Assert";
        case SDL_LOG_CATEGORY_SYSTEM:
            return "System";
        case SDL_LOG_CATEGORY_AUDIO:
            return "Audio";
        case SDL_LOG_CATEGORY_VIDEO:
            return "Video";
        case SDL_LOG_CATEGORY_RENDER:
            return "Render";
        case SDL_LOG_CATEGORY_INPUT:
            return "Input";
        case SDL_LOG_CATEGORY_TEST:
            return "Test";
        default:
            if(category > SDL_LOG_CATEGORY_TEST && category < SDL_LOG_CATEGORY_CUSTOM)
                return "Reserved";
            else if(category >= SDL_LOG_CATEGORY_CUSTOM)
                return "Custom";
        };
        return "Unknown";
    };
    void RedirectSDLOutput(bool redirect)
    {
        void* tag = (void*)__func__;

        if(redirect == false)
        {
            void* user = nullptr;
            SDL_LogGetOutputFunction(nullptr, &user);
            if(user == tag)
                SDL_LogSetOutputFunction(nullptr, nullptr);
            return;
        }

        SDL_LogSetOutputFunction(
            [](void*, int category, SDL_LogPriority priority, const char* message)
            {
                #define SR_TRACE_DO_LOG(lvl) BOOST_LOG_TRIVIAL(lvl)\
                    << "[SDL2_Log : " << to_chars(category) << "]\n"\
                    << message

                switch(priority)
                {
                case SDL_LOG_PRIORITY_VERBOSE:
                    SR_TRACE_DO_LOG(trace);
                    break;
                case SDL_LOG_PRIORITY_DEBUG:
                    SR_TRACE_DO_LOG(debug);
                    break;
                case SDL_LOG_PRIORITY_INFO:
                    SR_TRACE_DO_LOG(info);
                    break;
                case SDL_LOG_PRIORITY_WARN:
                    SR_TRACE_DO_LOG(warning);
                    break;
                case SDL_LOG_PRIORITY_ERROR:
                    SR_TRACE_DO_LOG(error);
                    break;
                default: // Treat unknown priority as critical
                case SDL_LOG_PRIORITY_CRITICAL:
                    SR_TRACE_DO_LOG(fatal);
                    break;
                }
            },
            tag
        );
    }
} // namespace srose::trace
