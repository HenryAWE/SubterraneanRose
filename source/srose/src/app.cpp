/**
 * @file app.cpp
 * @author HenryAWE
 * @brief Application
 */

#include <sr/srose/app.hpp>
#include <fstream>
#include <sr/filesystem/filesystem.hpp>


namespace srose
{
    Application& GetApp()
    {
        static Application instance;
        return instance;
    }

    Application::Application()
    {

    }

    Application::~Application() noexcept
    {

    }

    void Application::LoadUsers()
    {
        if(!filesystem::exists("./user.dat"))
            return;

        std::ifstream ifs("./users.dat");
        while(ifs)
        {
            m_users.emplace(ifs);
        }
    }
} // namespace srose
