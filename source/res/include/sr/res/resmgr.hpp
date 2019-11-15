/**
 * @file resmgr.hpp
 * @author HenryAWE
 * @brief Resource manager
 */

#ifndef SROSE_RES_resmgr_hpp_
#define SROSE_RES_resmgr_hpp_

#include "config.h"
#include <sr/filesystem/filesystem.hpp>
#include <sr/gpu/image.hpp>


namespace srose::res
{
    /**
     * @brief Resource Manager
     */
    class ResourceManager
    {
        filesystem::path m_res_path; // Data path
    public:
        ResourceManager();
        ResourceManager(const ResourceManager&) = delete;

        [[nodiscard]]
        /**
         * @brief Get the resource data path
         * 
         * @return const filesystem::path& Data path
         */
        const filesystem::path& GetPath() const noexcept { return m_res_path; }
    };
} // namespace srose::res


#endif
