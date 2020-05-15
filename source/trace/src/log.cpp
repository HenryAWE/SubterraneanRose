/**
 * @file log.cpp
 * @author HenryAWE
 * @brief Logging
 */

#include <sr/trace/log.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <SDL.h>


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

    const char* to_chars(SDL_LogCategory category)
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
                    << "[SDL2_Log : " << to_chars((SDL_LogCategory)category) << "]\n"\
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
