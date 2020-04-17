/**
 * @file srlc_editor.hpp
 * @author HenryAWE
 * @brief SRLC editor
 */

#ifndef SROSE_UI_GUI_EDITOR_srlc_editor_hpp_
#define SROSE_UI_GUI_EDITOR_srlc_editor_hpp_

#include <sr/ui/gui/widget.hpp>
#include <sr/util/string_tree.hpp>


namespace srose::ui::editor
{
    class SrlcEditor : public Widget
    {
    public:
        SrlcEditor();

        ~SrlcEditor();

        void Update() override;

        bool open = true;

    private:
        std::string m_title;
        std::string m_null;

        void Load();
        void OnImbue() override;

        std::unique_ptr<util::string_tree<std::string>> m_tree;
        void IterateTree(util::string_tree<std::string>& tr);
    };
} // namespace srose::ui::editor


#endif
