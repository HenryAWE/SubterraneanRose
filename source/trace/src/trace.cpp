/**
 * @file trace.hpp
 * @author HenryAWE
 * @brief Runtime debugging and stack tracing
 */

#include <sr/trace/trace.hpp>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <SDL.h>
#include <sr/trace/log.hpp>
#include <fmt/core.h>


namespace srose::trace
{
    void OutputStackAndQuickExit(int sig) noexcept
    {
        std::signal(sig, SIG_DFL);
        try
        {
            std::string rec;
            {
                auto trace = boost::stacktrace::stacktrace();
                std::stringstream ss;
                ss << trace;
                rec = ss.str();
            }
            const char* signame = "Unknown signal";
            switch(sig)
            {
                #define SR_GETSIGNAME(SIG) case SIG: signame = #SIG; break
                SR_GETSIGNAME(SIGINT);
                SR_GETSIGNAME(SIGILL);
                SR_GETSIGNAME(SIGFPE);
                SR_GETSIGNAME(SIGABRT);
                SR_GETSIGNAME(SIGSEGV);
                #undef SR_GETSIGNAME
            }

            BOOST_LOG_TRIVIAL(fatal)
                << fmt::format("{} - {}\n{}", signame, sig, rec);

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                signame,
                rec.c_str(),
                nullptr
            );
        }
        catch(...){}
        std::quick_exit(3);
    }

    void RegisterSignalHandler() noexcept
    {
        for(int sig : { SIGILL, SIGSEGV, SIGABRT })
        {
            std::signal(sig, OutputStackAndQuickExit);
        }
    }
} // namespace srose::trace
