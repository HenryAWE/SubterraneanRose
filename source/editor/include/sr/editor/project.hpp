/**
 * @file project.hpp
 * @author HenryAWE
 * @brief Project
 */

#ifndef SROSE_EDITOR_project_hpp_
#define SROSE_EDITOR_project_hpp_

#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include "config.hpp"


namespace srose::editor
{
    class Project
    {
        std::string m_name;
        std::string m_description;
    public:
        Project();
        Project(const Project& other);
        Project(Project&& move);

        ~Project() noexcept;

        #define SR_PROJECT_PROP(funcname, varname)\
        /* get */\
        [[nodiscard]]\
        const decltype(varname)& funcname() noexcept { return varname; }\
        /* set */\
        void funcname(const decltype(varname)& var) { varname = var; }

        SR_PROJECT_PROP(name, m_name)
        SR_PROJECT_PROP(description, m_description)

        /**
         * @brief Duplicate project
         *
         * @param inc Increasing number of the name if the lastest part of the original name is number
         * @return Project New project instance
         */
        Project Duplicate(bool inc = true);

    private:
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_name;
            ar & m_description;
        }
    };
} // namespace srose::editor


#endif
