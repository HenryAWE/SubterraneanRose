/**
 * @file project.cpp
 * @author HenryAWE
 * @brief Project
 */

#include <sr/editor/project.hpp>
#include <regex>


namespace srose::editor
{
    Project::Project()
        : m_name("Subterranean Rose Editor Project") {}
    Project::Project(const Project& other) = default;
    Project::Project(Project&& move) = default;

    Project::~Project() noexcept = default;

    Project Project::Duplicate(bool inc)
    {
        if(inc == false)
            return *this;
        Project dup(*this);
        std::regex re(R"(^(.*)\.(\d+)$)");
        if(std::regex_match(m_name, re))
        {
           int num = std::atoi(std::regex_replace(m_name, re, "$2").c_str());
           dup.m_name = std::regex_replace(m_name, re, "$1."+std::to_string(++num));
        }
        else
        {
            dup.m_name += ".1";
        }

        return std::move(dup);
    }
} // namespace srose::editor
