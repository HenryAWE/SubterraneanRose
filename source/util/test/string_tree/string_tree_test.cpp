/*Test the StringTree */

#define BOOST_TEST_MODULE string_tree_test
#include <boost/test/included/unit_test.hpp>
#include <sr/util/string_tree.hpp>
#include <iostream>

using namespace std;
using namespace srose::util;

BOOST_AUTO_TEST_CASE(test_string_path)
{
    basic_string_path<char, '.'> sp("test.for.path");
    auto it = sp.begin();
    BOOST_TEST_REQUIRE(bool(it == sp.begin()));
    auto test = *it;
    BOOST_TEST_REQUIRE(bool(test == "test"));
    auto for_ = *++it;
    BOOST_TEST_REQUIRE(bool(for_ == "for"));
    auto end_ = *++it;
    BOOST_TEST_REQUIRE(bool(end_ == "path"));
    BOOST_TEST_REQUIRE(!bool(it == sp.end()));
    BOOST_TEST_REQUIRE(bool(it != sp.end()));
    ++it;
    BOOST_TEST_REQUIRE(bool(it == sp.end()));

    const char* parsed[3] = {"test", "for", "path"};
    std::size_t idx = 0;
    for (std::string_view sv : sp)
    {
        BOOST_TEST_REQUIRE(bool(sv == parsed[idx++]));
    }
}

BOOST_AUTO_TEST_CASE(test_string_tree_ctor)
{
    string_tree<int, '/'> base;
    base.emplace_at("srose/value/1", 233);
    string_tree<int, '/'> copied(base);
    BOOST_TEST_REQUIRE(copied.get_value("srose/value/1") == 233);

    auto moved(std::move(base));
    BOOST_TEST_REQUIRE(base.size() == 0);
    BOOST_TEST_REQUIRE(moved.get_value("srose/value/1") == 233);
}

BOOST_AUTO_TEST_CASE(test_string_tree)
{
    // Test emplacement
    string_tree<int, '/'> st(5);
    static_assert(st.separator() == '/' ,"Unexpected separator");
    BOOST_TEST_REQUIRE(st.get_value() == 5);
    st.emplace(233);
    BOOST_TEST_REQUIRE(st.has_value());
    BOOST_TEST_REQUIRE(st.get_value() == 233);
    st.emplace(666);
    BOOST_TEST_REQUIRE(st.get_value() == 666);

    // Test operators
    st.emplace_at("first/second", 12);
    st.emplace_at("first", 1);
    BOOST_TEST_REQUIRE(st["first"] == 1);
    BOOST_TEST_REQUIRE(st["first/second"] == 12);
    st["first"] = 21;
    BOOST_TEST_REQUIRE(st.get_value("first") == 21);
    *st = 4;
    BOOST_TEST_REQUIRE(*st == 4);
    st.modify([](int& v){ v=6; });
    BOOST_TEST_REQUIRE((*const_cast<const string_tree<int, '/'>&>(st) == 6));

    // Test assignment
    std::string tm = "move";
    string_tree<std::string> sts = std::move(tm);
    BOOST_TEST_REQUIRE(tm.empty());
    BOOST_TEST_REQUIRE(sts.get_value() == "move");
    std::string tt = "koishi";
    sts.emplace(tt.begin(), tt.end());
    BOOST_TEST_REQUIRE(sts.get_value() == "koishi");

    // Test modifying
    sts.emplace_at("bs.ss", "bsss"s);
    BOOST_TEST_REQUIRE(sts.size() == 1);
    BOOST_TEST_REQUIRE(sts.get_value_optional("bs").has_value() == false);
    BOOST_TEST_REQUIRE(sts.get_value("bs.ss") == "bsss");
    sts.modify("bs.ss", [](auto& in){ in += "-modified"; });
    BOOST_TEST_REQUIRE(sts.get_value("bs.ss") == "bsss-modified");
    BOOST_TEST_REQUIRE(sts.has_value("never.existed") == false);
    BOOST_TEST_REQUIRE(sts.has_value("bs.ss"));

    string_tree<string> sts2;
    sts2.emplace_at("bs.ss" ,"not true");
    sts2.emplace_at("ss.ss", "true");

    sts.merge(sts2);
    BOOST_TEST_REQUIRE(sts.get_value("ss.ss") == "true");
    BOOST_TEST_REQUIRE(sts.get_value("bs.ss") != "not true");
    BOOST_TEST_REQUIRE(sts.has_value("bs.ss") == true);
    sts.emplace("value");
    BOOST_TEST_REQUIRE(sts.get_value() == "value");
    sts.erase();
    BOOST_TEST_REQUIRE(sts.has_value() == false);
    sts.erase_at("bs.ss");
    BOOST_TEST_REQUIRE(sts.has_value("bs.ss") == false);
    sts.erase_at("never.throw");

    try
    {
        (void)sts.get_value("must.throw.exception");
        BOOST_TEST_FAIL("Shoundn't reach this line");
    }
    catch (const string_tree_base::path_not_found&) {}
}

