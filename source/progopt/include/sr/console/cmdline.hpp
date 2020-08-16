/**
 * @file cmdline.hpp
 * @author HenryAWE
 * @brief Command line interface
 */

#ifndef SROSE_PROGOPT_cmdline_hpp_
#define SROSE_PROGOPT_cmdline_hpp_

#include <iosfwd>
#include <string>
#include <memory>
#include <optional>


namespace srose::progopt
{
    namespace detailed
    {
        class CLIData;
    } // namespace detailed

    /**
     * @brief CLI
     */
    class CommandLineInterface
    {
        std::unique_ptr<detailed::CLIData> m_clidata;
        bool m_quit_req = false;
        bool m_win_pause_req = false;
        int m_argc = 0;
        char** m_argv = nullptr;
    public:
        CommandLineInterface();
        explicit CommandLineInterface(std::ostream& os);
        CommandLineInterface(const CommandLineInterface&) = delete;

        ~CommandLineInterface();

        /**
         * @brief Get the default global instance of the CLI class
         */
        static CommandLineInterface& GetGlobalInstance();

        bool ParseArg(int argc, char* argv[]);
        void HandleArg();

        bool Exists(const std::string& name);
        std::size_t Count(const std::string& name);
        std::optional<bool> GetBool(const std::string& name);
        std::optional<int> GetInt(const std::string& name);
        std::optional<std::string> GetString(const std::string& name);

        void WinRequestOutput(bool force = false);

        void RequestQuit(bool req = true) noexcept { m_quit_req = req; }
        bool QuitRequested() const noexcept { return m_quit_req; }
        void WinRequestPause(bool req = true) noexcept { m_win_pause_req = req; }
        bool WinPauseRequested() const noexcept;

        std::ostream& GetOutputStream();

        // Helpers
        std::string GenerateHelp();

        // Functions for retrieving parsed data
        std::string GetPreferredLanguage();
        bool FullscreenRequired();
        bool VSyncRequired();
    };
} // namespace srose::progopt


#endif
