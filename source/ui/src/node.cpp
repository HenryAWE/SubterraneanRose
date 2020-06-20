/**
 * @file node.cpp
 * @author HenryAWE
 * @brief UI element node
 */

#include <sr/ui/node.hpp>
#include <imgui.h>
#include <imguisr.h>


namespace srose::ui
{
    // Base node
    BaseNode::BaseNode() = default;
    BaseNode::~BaseNode() = default;

    void BaseNode::Update() {}


    // Root node
    RootNode::RootNode(std::string_view id)
        : m_id("###" + std::string(id)) {}

    void RootNode::Update()
    {
        Base::Update();

        auto& io = ImGui::GetIO();

        constexpr int background_flags =
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
        ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            m_id.c_str(),
            nullptr,
            background_flags
        );
        if(!background)
            return;
    }


    // Stand-alone node
    StandaloneNode::StandaloneNode(std::string title, std::string name)
    {
        SetId(std::move(title), std::move(name));
    }

    void StandaloneNode::Update()
    {
        if(!m_open)
            return;
        auto guard = BeginContext();
        if(!guard)
            return;
    }

    StandaloneNode::ContextGuard::~ContextGuard() noexcept
    {
        node.EndContext();
    }

    StandaloneNode::ContextGuard StandaloneNode::BeginContext()
    {
        bool value = ImGui::Begin(m_id.c_str(), &m_open);
        return ContextGuard(*this, value);
    }

    void StandaloneNode::EndContext() noexcept
    {
        ImGui::End();
    }

    void StandaloneNode::SetId(std::string title)
    {
        m_title.swap(title);
        m_id = m_title + "###" + m_name;
    }
    void StandaloneNode::SetId(std::string title, std::string name)
    {
        m_title.swap(title);
        m_name.swap(name);
        m_id = m_title + "###" + m_name;
    }


    // I18n node
    I18nNode::I18nNode()
        : I18nNode(std::locale()) {}
    I18nNode::I18nNode(const std::locale& loc)
        : I18nNode(std::use_facet<locale::TranslationFacet>(loc).getptr()) {}
    I18nNode::I18nNode(std::shared_ptr<locale::Language> lang)
        : m_lang(std::move(lang)) {}

    I18nNode::~I18nNode()
    {
        m_connection.disconnect();
    }
    
    void I18nNode::Update()
    {
        Base::Update();
    }

    void I18nNode::SetLanguage(const std::locale& loc)
    {
        SetLanguage(std::use_facet<locale::TranslationFacet>(loc).getptr());
    }
    void I18nNode::SetLanguage(std::shared_ptr<locale::Language> lang)
    {
        m_lang.swap(lang);
    }

    void I18nNode::Connect(boost::signals2::signal<void(const std::locale&)>& notifier)
    {
        m_connection = notifier.connect(
            [this](const std::locale& loc) { SetLanguage(loc); }
        );
    }

    std::string I18nNode::gettext(std::string_view id)
    {
        return m_lang->gettext(id);
    }

    void I18nNode::LoadI18nData() {}
} // namespace srose::ui
