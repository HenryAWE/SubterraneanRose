/**
 * @file node_test.cpp
 * @author HenryAWE
 * @brief Test UI element node
 */

#define BOOST_TEST_MODULE node_test
#include <boost/test/included/unit_test.hpp>
#include <sr/ui/node.hpp>
#include <sr/i18n/i18n.hpp>


BOOST_AUTO_TEST_CASE(test_node)
{
    using namespace srose;
    using namespace srose::ui;

    auto i18n_node = std::make_shared<I18nNode>(i18n::GetDefaultLanguage());
    
    BOOST_TEST(i18n_node->getptr()->GetId() == "C");
}
