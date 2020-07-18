/**
 * @file cmdline.cpp
 * @author HenryAWE
 * @brief Functions for parsed command line arguments
 */

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
            std::optional<po::options_description> desc;
            std::shared_ptr<locale::Language> language;

            CLIData(std::ostream* os, std::shared_ptr<locale::Language> language_)
                : output(os), language(std::move(language_)) {}

            po::options_description BuildDesc()
            {
                constexpr unsigned int line_length = UINT_MAX;

                auto _ = [this](std::string_view path) { return language->gettext(path); };

                po::options_description generic(_("srose.cui.generic"), line_length);
                generic.add_options()
                    ("help", _("srose.cui.generic.help").c_str())
                    ("version", _("srose.cui.generic.version").c_str())
                    ("build-info", _("srose.cui.generic.build").c_str());

                po::options_description language(_("srose.cui.lang"), line_length);
                language.add_options()
                    ("language", po::value<std::string>()->value_name("name")->default_value("auto"), _("srose.cui.lang.language").c_str())
                    ("available-language", _("srose.cui.lang.available").c_str());

                po::options_description display(_("srose.cui.display"), line_length);
                display.add_options()
                    ("fullscreen,F", _("srose.cui.display.fullscreen").c_str())
                    ("vsync,V", "Enable vertical synchronization");
                
                po::options_description video(_("srose.cui.video"), line_length);
                video.add_options()
                    ("get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), _("srose.cui.get-display-mode").c_str());

                #ifdef __WINDOWS__
                po::options_description win32("Windows", line_length);
                win32.add_options()
                    ("win-console", po::value<std::string>()->value_name("mode")->default_value("auto"), "Windows console");
                #endif

                return po::options_description(_("srose.cui.total"), line_length)
                    .add(generic)
                #ifdef __WINDOWS__
                    .add(win32)
                #endif
                    .add(language)
                    .add(display)
                    .add(video);
            }

            void ParseArg(int argc, char* argv[])
            {
                if(!desc)
                    desc.emplace(BuildDesc());
                po::store(po::parse_command_line(argc, argv, *desc), vm);
                po::notify(vm);
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
        m_clidata->ParseArg(argc, argv);
    }
    void CommandLineInterface::HandleArg()
    {
        using std::endl;

        auto& os = *m_clidata->output;
        auto& vm = m_clidata->vm;
        detailed::RequestCommandLineOutput(vm);

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

    bool CommandLineInterface::WinPauseRequested() const noexcept
    {
        #ifdef BOOST_WINDOWS
        return m_win_pause_req && detailed::win_helper.release;
        #else
        return false;
        #endif
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
