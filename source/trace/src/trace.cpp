/**
 * @file trace.hpp
 * @author HenryAWE
 * @brief Runtime debugging and stack tracing
 */

#include <sr/trace/trace.hpp>
#include <fstream>
#include <cstdlib>
#include <csignal>
#include <SDL.h>


namespace srose::trace
{
    void OutputStackAndQuickExit(int sig) noexcept
    {
        std::signal(sig, SIG_DFL);
        try
        {
            auto rec = to_string(boost::stacktrace::stacktrace());
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

            SDL_LogCritical(
                SDL_LOG_CATEGORY_APPLICATION,
                "[trace] %s - %d\n%s",
                signame, sig,
                rec.c_str()
            );

            std::ofstream dump("sr-dump.txt");
            if(dump.good())
            {
                dump << rec << std::endl;
                dump.close();
            }

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
