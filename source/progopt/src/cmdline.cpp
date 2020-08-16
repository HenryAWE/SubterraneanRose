/**
 * @file cmdline.cpp
 * @author HenryAWE
 * @brief Command line interface
 */

#ifndef SROSE_DISABLE_CLI

#include <sr/console/cmdline.hpp>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <fmt/core.h>
#include <sr/core/version_info.hpp>
#include <sr/util/system.hpp>
#include <sr/locale/locale.hpp>
#include <sr/i18n/i18n.hpp>
#include "win32_console.hpp"
#include "buildinfo.hpp"


namespace srose::progopt
{
    namespace po = boost::program_options;

    namespace detailed
    {
        #ifdef BOOST_WINDOWS
        WinConsoleMode ParseWinConsoleMode(
            std::string_view sv,
            WinConsoleMode fallback
        ) noexcept {
            if(sv == "auto") return SR_WINCLI_AUTO;
            else if(sv == "alloc" || sv == "new") return SR_WINCLI_ALLOC;
            else if(sv == "attach") return SR_WINCLI_ATTACH;
            else if(sv == "ignore") return SR_WINCLI_IGNORE;

            return fallback;
        }

        struct WinConsoleHostHelper
        {
            bool release = false;
            bool alloc = false;

            WinConsoleHostHelper() noexcept = default;
            ~WinConsoleHostHelper() noexcept
            {
                if(release) ReleaseConsoleWin32();
            }
        };
        static WinConsoleHostHelper win_helper;

        static void RequestConsole(WinConsoleMode wincli_mode, bool fallback)
        {
            if(wincli_mode == SR_WINCLI_ALLOC)
                win_helper.alloc = win_helper.release = AllocConsoleWin32();
            else if(wincli_mode == SR_WINCLI_ATTACH)
                win_helper.release = AttachConsoleWin32();
            else if(wincli_mode == SR_WINCLI_AUTO)
            {
                wincli_mode = ParseWinConsoleMode(
                    util::GetEnv("SROSE_WIN_CONSOLE").value_or("auto"),
                    SR_WINCLI_AUTO
                );

                if(wincli_mode == SR_WINCLI_ALLOC)
                {
                    win_helper.alloc = win_helper.release = AllocConsoleWin32();
                    return;
                }
                else if(wincli_mode == SR_WINCLI_ATTACH)
                {
                    win_helper.release = AttachConsoleWin32();
                    return;
                }
                else if(wincli_mode == SR_WINCLI_IGNORE)
                    return;

                // wincli_mode == SR_WINCLI_AUTO
                win_helper.release = AttachConsoleWin32();
                if(!win_helper.release && fallback)
                    win_helper.alloc = win_helper.release = AllocConsoleWin32();
                }
        }
        #else
        WinConsoleMode ParseWinConsoleMode(
            std::string_view sv,
            WinConsoleMode fallback
        ) noexcept {
            (void)sv;
            return fallback;
        }
        static void RequestConsole(WinConsoleMode wincli_mode, bool fallback) {}
        #endif

        class CLIData
        {
        public:
            std::ostream* output = nullptr;
            po::variables_map vm;
            std::vector<std::string> unrecognized;
            std::optional<po::options_description> desc;
            std::shared_ptr<locale::Language> language;

            CLIData(std::ostream* os, std::shared_ptr<locale::Language> language_)
                : output(os), language(std::move(language_)) {}

            po::options_description BuildDesc()
            {
                constexpr unsigned int line_length = UINT_MAX;

                auto _ = [this](std::string_view path) { return language->GetTextWith(path, locale::SRLC_RETURN_REQUEST); };

                po::options_description generic(_("srose.cli.generic"), line_length);
                generic.add_options()
                    ("help,?", po::bool_switch(), _("srose.cli.generic.help").c_str())
                    ("version", po::bool_switch(), _("srose.cli.generic.version").c_str())
                    ("build-info", po::bool_switch(), _("srose.cli.generic.build").c_str())
                    ("explore-appdata,E", po::bool_switch(), _("srose.cli.generic.explore-appdata").c_str())
                    ("print-appdata",po::bool_switch(),  _("srose.cli.generic.print-appdata").c_str());

                po::options_description language(_("srose.cli.lang"), line_length);
                language.add_options()
                    ("language,L", po::value<std::string>()->value_name("name")->default_value("auto"), _("srose.cli.lang.language").c_str())
                    ("lang-available", po::bool_switch(), _("srose.cli.lang.available").c_str());

                po::options_description display(_("srose.cli.display"), line_length);
                display.add_options()
                    ("display-fullscreen,F", po::bool_switch(), _("srose.cli.display.fullscreen").c_str())
                    ("display-vsync,V", po::bool_switch(), _("srose.cli.display.vsync").c_str());
                
                po::options_description video(_("srose.cli.video"), line_length);
                video.add_options()
                    ("video-get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), _("srose.cli.video.get-display-mode").c_str());

                #ifdef __WINDOWS__
                po::options_description win32(_("srose.cli.win").c_str(), line_length);
                win32.add_options()
                    ("win-console", po::value<std::string>()->value_name("mode")->default_value("auto"), _("srose.cli.win.console").c_str());
                #endif

                return po::options_description(_("srose.cli.total"), line_length)
                    .add(generic)
                #ifdef __WINDOWS__
                    .add(win32)
                #endif
                    .add(language)
                    .add(display)
                    .add(video);
            }

            bool ParseArg(int argc, char* argv[], CommandLineInterface& cli)
            {
                if(!desc)
                    desc.emplace(BuildDesc());

                auto& os = *output;
                try
                {
                    auto parsed = po::command_line_parser(argc, argv)
                        .options(*desc)
                        .allow_unregistered()
                        .run();
                    po::store(parsed, vm);
                    po::notify(vm);

                    unrecognized = po::collect_unrecognized(parsed.options, po::include_positional);
                }
                catch(const po::error& e)
                {
                    detailed::RequestConsole(cli.m_wincli_mode, true);
                    cli.OutputError(e.what());
                    os << std::endl;
                    return false;
                }

                return true;
            }

            void UpdateI18nData(std::shared_ptr<locale::Language> new_lang)
            {
                if(new_lang == language)
                    return;
                language.swap(new_lang);
                desc.emplace(BuildDesc());
            }
        };

        template <typename T>
        std::optional<T> GetValueImpl(po::variables_map& vm, const std::string& name)
        {
            try
            {
                return vm.at(name).as<T>();
            }
            catch(...) {}

            return std::nullopt;
        }
    } // namespace detailed

    CommandLineInterface::CommandLineInterface()
        : CommandLineInterface(std::cout) {}
    CommandLineInterface::CommandLineInterface(std::ostream& os)
        #ifdef _WIN32
        : m_wincli_mode(SR_WINCLI_AUTO),
        #else
        : m_wincli_mode(SR_WINCLI_IGNORE),
        #endif
        m_clidata(std::make_unique<detailed::CLIData>(
            &os,
            std::has_facet<locale::TranslationFacet>(std::locale()) ?
                std::use_facet<locale::TranslationFacet>(std::locale()).getptr() :
                nullptr
        )) {}

    CommandLineInterface::~CommandLineInterface() = default;

    CommandLineInterface& CommandLineInterface::GetGlobalInstance()
    {
        static CommandLineInterface instance(std::cout);
        return instance;
    }

    bool CommandLineInterface::ParseArg(int argc, char* argv[])
    {
        m_argc = argc;
        m_argv = argv;
        bool no_error = m_clidata->ParseArg(argc, argv, *this);
        if(!no_error)
        {
            RequestQuit();
            WinRequestPause();
        }

        return no_error;
    }
    void CommandLineInterface::HandleArg()
    {
        using std::endl;

        auto& os = *m_clidata->output;
        auto& vm = m_clidata->vm;

        m_wincli_mode = detailed::ParseWinConsoleMode(
            GetString("win-console").value_or("auto"),
            SR_WINCLI_AUTO
        );

        if(auto& unrecognized = m_clidata->unrecognized; !unrecognized.empty())
        {
            detailed::RequestConsole(m_wincli_mode, true);
            std::stringstream ss;
            ss << "Unrecognized option(s): ";
            for(const auto& i : unrecognized)
                ss << fmt::format("\"{}\" ", i);
            ss << std::endl;
            OutputError(ss.rdbuf());

            RequestQuit();
            WinRequestPause();
            return;
        }

        std::string preferred = GetPreferredLanguage();
        if(!preferred.empty())
        {
            i18n::SelectLanguage(preferred.c_str());
            m_clidata->UpdateI18nData(i18n::GetNearestLanguage(preferred));
        }
        else
        {
            i18n::SelectLanguage(nullptr);
        }
        os.imbue(std::locale());

        if(GetBool("help").value_or(false))
        {
            WinRequestConsole(true);
            os << "Subterranean Rose CLI\n" << GenerateHelp() << endl;
            WinRequestPause();
            RequestQuit();
        }
        if(GetBool("version").value_or(false))
        {
            WinRequestConsole(true);
            os
                << fmt::format(
                        "{}.{}.{} - {}",
                        SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH,
                        core::GitCommitShortID()
                    )
                << std::endl;
            WinRequestPause();
            RequestQuit();
        }
        if(GetBool("build-info").value_or(false))
        {
            WinRequestConsole(true);
            os
                << fmt::format(
                    "Subterranean Rose {}\n"
                    "{} - {}",
                    core::GetVersionString(),
                    core::GitCommitMsg(), core::GitCommitID()
                )
                << std::endl;

            const char* commit_body = core::GitCommitBody();
            if(commit_body[0] != '\0')
            {
                os << std::endl << commit_body << std::endl;
            }

            os << std::endl << "Branch: " << core::GitBranch() << std::endl;

            const char* build_suffix = core::GetBuildSuffix();
            if(build_suffix[0] != '\0')
            {
                os << std::endl << "Build suffix: " <<  build_suffix << std::endl;
            }

            os << std::endl << std::endl;

            /* C++ Information */
            os
                << "C++ Information" << std::endl
                << GenerateCppInfo()
                << std::endl;
            /* Third Party Libraries */
            os
                << "Third Party Libraries" << std::endl
                << GenerateLibInfo()
                << std::endl;

            WinRequestPause();
            RequestQuit();
        }
    }

    bool CommandLineInterface::Exists(const std::string& name)
    {
        return m_clidata->vm.count(name) > 0;
    }
    std::size_t CommandLineInterface::Count(const std::string& name)
    {
        return m_clidata->vm.count(name);
    }
    std::optional<bool> CommandLineInterface::GetBool(const std::string& name)
    {
        return detailed::GetValueImpl<bool>(m_clidata->vm, name);
    }
    std::optional<int> CommandLineInterface::GetInt(const std::string& name)
    {
        return detailed::GetValueImpl<int>(m_clidata->vm, name);
    }
    std::optional<std::string> CommandLineInterface::GetString(const std::string& name)
    {
        return detailed::GetValueImpl<std::string>(m_clidata->vm, name);
    }

    // static version
    void CommandLineInterface::WinRequestConsole(WinConsoleMode mode, bool fallback)
    {
        detailed::RequestConsole(mode, fallback);
    }
    void CommandLineInterface::WinRequestConsole(bool fallback)
    {
        // Use the parsed data to invoke the static version
        WinRequestConsole(m_wincli_mode, fallback);
    }

    bool CommandLineInterface::WinPauseRequested() const noexcept
    {
        #ifdef BOOST_WINDOWS
        return m_win_pause_req && detailed::win_helper.alloc;
        #else
        return false;
        #endif
    }

    std::ostream& CommandLineInterface::GetOutputStream()
    {
        return *m_clidata->output;
    }

    std::string CommandLineInterface::GenerateHelp()
    {
        std::stringstream ss;
        if(m_clidata->desc)
            ss << *m_clidata->desc;
        return ss.str();
    }

    std::string CommandLineInterface::GetPreferredLanguage()
    {

        std::string lang = GetString("language").value_or("auto");
        return lang == "auto" ? std::string() : std::move(lang);
    }
    bool CommandLineInterface::FullscreenRequired()
    {
        return GetBool("display-fullscreen").value_or(false);
    }
    bool CommandLineInterface::VSyncRequired()
    {
        return GetBool("display-vsync").value_or(false);
    }
} // namespace srose::progopt

#else

#include <sr/console/cmdline.hpp>
#include <iostream>
#include <stdexcept>
#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#endif


namespace srose::progopt
{
    namespace detailed
    {
        class CLIData
        {
        public:
            std::ostream* output = nullptr;

            CLIData(std::ostream* os) noexcept : output(os) {}
            ~CLIData() noexcept = default;
        };

        [[noreturn]]
        static void Unimplemented()
        {
            throw std::runtime_error("[Console] Unimplemented");
        }
    } // namespace detailed
    

    CommandLineInterface::CommandLineInterface()
        :CommandLineInterface(std::cout) {}
    CommandLineInterface::CommandLineInterface(std::ostream& os)
        : m_clidata(std::make_unique<detailed::CLIData>(&os)) {}

    CommandLineInterface::~CommandLineInterface() = default;

    CommandLineInterface& CommandLineInterface::GetGlobalInstance()
    {
        static CommandLineInterface instance(std::cout);
        return instance;
    }

    void CommandLineInterface::ParseArg(int argc, char* argv[])
    {
        m_argc = argc;
        m_argv = argv;
    }
    void CommandLineInterface::HandleArg()
    {
        if(m_argc > 1)
        {
            bool quit = true;
            const char* title = "The command line interface has been disabled";
            auto msg = GenerateHelp();
            #ifdef _WIN32
            msg += "\n\nContinue?";
            quit = IDCANCEL == ::MessageBoxA(
                    nullptr,
                    msg.c_str(),
                    title,
                    MB_OKCANCEL | MB_ICONINFORMATION
            );
            #else
            GetOutputStream()
                << "\033[1m" << title << "\033[0m" << std::endl
                << msg << std::endl;
            #endif

            if(quit)
                RequestQuit();
        }
    }

    bool CommandLineInterface::Exists(const std::string& name)
    {
        return false;
    }
    std::size_t CommandLineInterface::Count(const std::string& name)
    {
        return 0;
    }
    std::optional<bool> CommandLineInterface::GetBool(const std::string& name)
    {
        return false;
    }
    std::optional<int> CommandLineInterface::GetInt(const std::string& name)
    {
        return std::nullopt;
    }
    std::optional<std::string> CommandLineInterface::GetString(const std::string& name)
    {
        return std::nullopt;
    }

    // static version
    void CommandLineInterface::WinRequestConsole(WinConsoleMode mode, bool fallback) {}
    void CommandLineInterface::WinRequestConsole(bool fallback) {}

    bool CommandLineInterface::WinPauseRequested() const noexcept
    {
        #ifdef _WIN32
        return m_win_pause_req;
        #else
        return false;
        #endif
    }

    std::ostream& CommandLineInterface::GetOutputStream()
    {
        return *m_clidata->output;
    }

    std::string CommandLineInterface::GenerateHelp()
    {
        return
            "The program was built with SROSE_DISABLE_CLI set to ON\n"
            "If you want the functionality of the command line interface,\n"
            "please re-build the program with SROSE_DISABLE_CLI set to OFF";
    }

    std::string CommandLineInterface::GetPreferredLanguage()
    {
        return "auto";
    }
    bool CommandLineInterface::FullscreenRequired()
    {
        return false;
    }
    bool CommandLineInterface::VSyncRequired()
    {
        return false;
    }
} // namespace srose::progopt

#endif
