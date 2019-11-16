/**
 * @file resmgr.cpp
 * @author HenryAWE
 * @brief Resource manager
 */

#include <sr/res/resmgr.hpp>
#include <memory>


namespace srose::res
{
    ResourceManager::ResourceManager()
        : m_res_path("resource")
    {

    }

    static std::unique_ptr<ResourceManager> g_resmgr;

    ResourceManager* SRSCALL CreateResourceManager()
    {
        g_resmgr = std::make_unique<ResourceManager>();

        return g_resmgr.get();
    }
    void SRSCALL DestroyResourceManager()
    {
        g_resmgr.reset();
    }
    ResourceManager* SRSCALL GetResourceManager() noexcept
    {
        return g_resmgr.get();
    }
} // namespace srose::res
