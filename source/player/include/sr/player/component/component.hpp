/**
 * @file component.hpp
 * @author HenryAWE
 * @brief Components
 */

#ifndef SROSE_PLAYER_COMPONENT_component_hpp_
#define SROSE_PLAYER_COMPONENT_component_hpp_

#include <cassert>
#include <cstddef>
#include "../config.h"


namespace srose::player::component
{
    class BaseComponent
    {
    public:
        typedef std::size_t Family;

    protected:
        static Family m_family_counter;
    };

    template <typename Derived>
    class Component : public BaseComponent
    {
    public:
        Component()
        {
            Register();
        }

        [[nodiscard]]
        static Family GetFamily() { return Register(); }

    private:
        static Family Register()
        {
            static Family family = m_family_counter++;
            assert(family < SROSE_PLAYER_ECS_MAXCOMPONENTS);
            return family;
        }
    };
} // namespace srose::player::component


#endif
