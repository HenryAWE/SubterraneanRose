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
#include <sr/locale/locale.hpp>
#include <sr/i18n/i18n.hpp>
#include "mswin.hpp"
#include "buildinfo.hpp"


namespace srose::console
{
    namespace po = boost::program_options;

    namespace detailed
    {
        #ifdef BOOST_WINDOWS
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

        static void SRSCALL RequestCommandLineOutput(po::variables_map& vm, bool force = false)
        {
            if(win_helper.release == false)
            {
                std::string conmode_win32;
                try { conmode_win32 = vm["win-console"].as<std::string>(); }
                catch(const boost::bad_any_cast&) { conmode_win32 = "auto"; }

                if(conmode_win32 == "new")
                {
                    win_helper.alloc = win_helper.release = AllocConsoleWin32();
                }
                else if(conmode_win32 == "attach")
                {
                    win_helper.release = AttachConsoleWin32();
                }
                else if(conmode_win32 == "auto")
                {
                    win_helper.release = AttachConsoleWin32();
                    if(!win_helper.release && force)
                        win_helper.alloc = win_helper.release = AllocConsoleWin32();
                }
            }
        }
        #else
        static void SRSCALL RequestCommandLineOutput(po::variables_map& vm, bool force = false) {}
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

                auto _ = [this](std::string_view path) { return language->gettext(path); };

                po::options_description generic(_("srose.cli.generic"), line_length);
                generic.add_options()
                    ("help", _("srose.cli.generic.help").c_str())
                    ("version", _("srose.cli.generic.version").c_str())
                    ("build-info", _("srose.cli.generic.build").c_str())
                    ("appdata", "Print the path of the data directory");

                po::options_description language(_("srose.cli.lang"), line_length);
                language.add_options()
                    ("language", po::value<std::string>()->value_name("name")->default_value("auto"), _("srose.cli.lang.language").c_str())
                    ("available-language", _("srose.cli.lang.available").c_str());

                po::options_description display(_("srose.cli.display"), line_length);
                display.add_options()
                    ("fullscreen,F", _("srose.cli.display.fullscreen").c_str())
                    ("vsync,V", "Enable vertical synchronization");
                
                po::options_description video(_("srose.cli.video"), line_length);
                video.add_options()
                    ("get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), _("srose.cli.get-display-mode").c_str());

                #ifdef __WINDOWS__
                po::options_description win32("Windows", line_length);
                win32.add_options()
                    ("win-console", po::value<std::string>()->value_name("mode")->default_value("auto"), "Windows console");
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

            bool ParseArg(int argc, char* argv[])
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
                    detailed::RequestCommandLineOutput(vm, true);
                    os << e.what() << std::endl;
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
    } // namespace detailed

    CommandLineInterface::CommandLineInterface()
        : CommandLineInterface(std::cout) {}
    CommandLineInterface::CommandLineInterface(std::ostream& os)
        : m_clidata(std::make_unique<detailed::CLIData>(
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

    void CommandLineInterface::ParseArg(int argc, char* argv[])
    {
        m_argc = argc;
        m_argv = argv;
        bool no_error = m_clidata->ParseArg(argc, argv);
        if(!no_error)
        {
            RequestQuit();
            WinRequestPause();
        }
    }
    void CommandLineInterface::HandleArg()
    {
        using std::endl;

        auto& os = *m_clidata->output;
        auto& vm = m_clidata->vm;
        detailed::RequestCommandLineOutput(vm);

        if(auto& unrecognized = m_clidata->unrecognized; !unrecognized.empty())
        {
            detailed::RequestCommandLineOutput(m_clidata->vm, true);
            os << "Unrecognized option(s): ";
            for(const auto& i : unrecognized)
                os << fmt::format("\"{}\" ", i);
            os << std::endl;
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

        if(vm.count("help"))
        {
            detailed::RequestCommandLineOutput(vm, true);
            os << "Subterranean Rose CLI\n" << GenerateHelp() << endl;
            WinRequestPause();
            RequestQuit();
        }
        if(vm.count("version"))
        {
            detailed::RequestCommandLineOutput(vm, true);
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
        if(vm.count("build-info"))
        {
            detailed::RequestCommandLineOutput(vm, true);
            os << fmt::format(
                "Subterranean Rose {}\n"
                "{} - {}\n"
                "Branch: {}\n",
                core::GetVersionString(),
                core::GitCommitMsg(), core::GitCommitID(),
                core::GitBranch()
            );
            const char* commit_body = core::GitCommitBody();
            if(commit_body[0] != '\0')
            {
                os << std::endl << commit_body << std::endl;
            }
            os << std::endl;

            const char* build_suffix = core::GetBuildSuffix();
            if(build_suffix[0] != '\0')
            {
                os << "Build suffix: " <<  build_suffix << std::endl;
            }
            os << std::endl;

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

    bool CommandLineInterface::WinPauseRequested() const noexcept
    {
        #ifdef BOOST_WINDOWS
        return m_win_pause_req && detailed::win_helper.release;
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
        if(m_clidata->vm.count("language"))
        {
            std::string lang = m_clidata->vm["language"].as<std::string>();
            return lang=="auto" ? std::string() : std::move(lang);
        }
        else
        {
            return std::string();
        }
    }
    bool CommandLineInterface::FullscreenRequired()
    {
        return m_clidata->vm.count("fullscreen");
    }
    bool CommandLineInterface::VSyncRequired()
    {
        return m_clidata->vm.count("vsync");
    }
} // namespace srose::console

#else

#include <sr/console/cmdline.hpp>
#include <iostream>
#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#endif


namespace srose::console
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
            auto help = GenerateHelp();
            #ifdef _WIN32
            ::MessageBoxA(
                    nullptr,
                    help.c_str(),
                    "The command line interface has been disabled",
                    MB_OK | MB_ICONINFORMATION
            );
            #else
            *m_clidata->output << help << std::endl;
            #endif

            RequestQuit();
        }
    }

    bool CommandLineInterface::WinPauseRequested() const noexcept
    {
        #ifdef _WIN32
        return m_win_pause_req;
        #else
        return false;
        #endif
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
        return std::string();
    }
    bool CommandLineInterface::FullscreenRequired()
    {
        return false;
    }
    bool CommandLineInterface::VSyncRequired()
    {
        return false;
    }
} // namespace srose::console

#endif
