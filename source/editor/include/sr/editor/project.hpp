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
    public:
        Project();

        ~Project() noexcept;

        #define SR_PROJECT_PROP(funcname, varname)\
        /* get */\
        [[nodiscard]]\
        const decltype(varname)& funcname() noexcept { return varname; }\
        /* set */\
        void funcname(const decltype(varname)& var) { varname = var; }

        SR_PROJECT_PROP(name, m_name)

    private:
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            using boost::serialization::make_nvp;
            ar & make_nvp("name", m_name);
        }
    };
} // namespace srose::editor


#endif
