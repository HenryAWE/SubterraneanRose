/**
 * @file cmdline.hpp
 * @author HenryAWE
 * @brief Command line interface
 */

#ifndef SROSE_PROGOPT_cmdline_hpp_
#define SROSE_PROGOPT_cmdline_hpp_

#include <iostream>
#include <string>
#include <memory>
#include <optional>


namespace srose::progopt
{
    namespace detailed
    {
        class CLIData;
    } // namespace detailed

    enum WinConsoleMode : int
    {
        SR_WINCLI_IGNORE = 0,
        SR_WINCLI_ATTACH,
        SR_WINCLI_ALLOC,
        SR_WINCLI_AUTO
    };

    /**
     * @brief Command-line interface (CLI)
     */
    class CommandLineInterface
    {
        friend class detailed::CLIData;
        std::unique_ptr<detailed::CLIData> m_clidata;
        WinConsoleMode m_wincli_mode;
        bool m_vtseq_avail = false; // Is virtual terminal sequence available
        bool m_quit_req = false;
        bool m_win_pause_req = false;
        int m_argc = 0;
        char** m_argv = nullptr;

        CommandLineInterface();
        explicit CommandLineInterface(std::ostream& os);
        CommandLineInterface(const CommandLineInterface&) = delete;

    public:
        ~CommandLineInterface();

        /**
         * @brief Get the default global instance of the CLI class
         */
        static CommandLineInterface& GetGlobalInstance();

        /**
         * @brief Parse the argument of the command line
         * 
         * @param argc Argument count
         * @param argv Argument values
         * @return Will return false if any error occurred 
         */
        bool ParseArg(int argc, char* argv[]);
        /**
         * @brief Handle some simple argument
         *
         * @return Will return false if any error occurred 
         */
        bool HandleArg();

        bool Exists(const std::string& name);
        std::size_t Count(const std::string& name);
        std::optional<bool> GetBool(const std::string& name);
        std::optional<int> GetInt(const std::string& name);
        std::optional<std::string> GetString(const std::string& name);

        void WinRequestConsole(bool fallback = false);

        void RequestQuit(bool req = true) noexcept { m_quit_req = req; }
        bool QuitRequested() const noexcept { return m_quit_req; }
        void WinRequestPause(bool req = true) noexcept { m_win_pause_req = req; }
        bool WinPauseRequested() const noexcept;

        /**
         * @brief Get the output stream of the CLI
         */
        std::ostream& GetOutputStream();

        // Helpers
        /**
         * @brief Generate help text
         */
        std::string GenerateHelp();

        // Output helpers
        template <typename T>
        void ColorizedOutput(const char* escseq, T&& value)
        {
            auto& os = GetOutputStream();
            if(m_vtseq_avail)
                os << escseq;
            os << value;
            if(m_vtseq_avail)
                os << "\033[0m";
        }
        template <typename T>
        void OutputWarning(T&& value)
        {
            ColorizedOutput("\033[33m", std::forward<T>(value));
        }
        template <typename T>
        void OutputError(T&& value)
        {
            ColorizedOutput("\033[1;31m", std::forward<T>(value));
        }

        // Functions for retrieving parsed data
        std::string GetPreferredLanguage();
        bool FullscreenRequired();
        bool VSyncRequired();
    };
} // namespace srose::progopt


#endif
