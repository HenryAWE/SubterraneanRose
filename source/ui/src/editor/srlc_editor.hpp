/**
 * @file srlc_editor.hpp
 * @author HenryAWE
 * @brief SRLC editor
 */

#ifndef SROSE_UI_GUI_EDITOR_srlc_editor_hpp_
#define SROSE_UI_GUI_EDITOR_srlc_editor_hpp_

#include <sr/ui/node.hpp>
#include <sr/util/string_tree.hpp>


namespace srose::ui::editor
{
    class SrlcEditor : public StandaloneNode
    {
    public:
        typedef StandaloneNode Base;

        SrlcEditor();

        ~SrlcEditor();

        void Update() override;

    private:
        std::string m_title;
        std::string m_null;

        void Load();
        void LoadI18nData() override;

        util::string_tree<std::string> m_tree;
        void IterateTree(util::string_tree<std::string>& tr);
    };
} // namespace srose::ui::editor


#endif
