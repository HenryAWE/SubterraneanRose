/**
 * @file app.hpp
 * @author HenryAWE
 * @brief Application
 */

#ifndef SROSE_SROSE_app_hpp_
#define SROSE_SROSE_app_hpp_

#include <set>
#include "config.hpp"
#include "user.hpp"


namespace srose
{
    class Application;

    [[nodiscard]]
    Application& GetApp();

    class Application final
    {
        friend Application& GetApp();

        Application();
        Application(const Application&) = delete;
    public:
        ~Application() noexcept;

        typedef std::set<UserProfile, UserProfile::Comparator> Users;

        void LoadUsers();

        [[nodiscard]]
        constexpr Users& GetUsers() noexcept { return m_users; }

    private:
        Users m_users;
    };
} // namespace srose


#endif
