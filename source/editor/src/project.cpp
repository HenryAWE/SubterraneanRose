/**
 * @file project.cpp
 * @author HenryAWE
 * @brief Project
 */

#include <sr/editor/project.hpp>


namespace srose::editor
{
    Project::Project()
        : m_name("Subterranean Rose Editor Project") {}

    Project::~Project() noexcept = default;
} // namespace srose::editor
