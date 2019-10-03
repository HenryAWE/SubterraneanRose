/*Test the StringTree */
#include <sr/util/string_tree.hpp>
#include <cassert>
#include <iostream>


int main()
{
    using namespace std;
    using namespace srose::util;

    // Test emplacement
    string_tree<int, '/'> st(5);
    static_assert(st.separator() == '/' ,"Unexpected separator");
    assert(st.get_value() == 5);
    st.emplace(233);
    assert(st.has_value());
    assert(st.get_value() == 233);
    st.emplace(666);
    assert(st.get_value() == 666);

    // Test assignment
    std::string tm = "move";
    string_tree<std::string> sts = std::move(tm);
    assert(tm.empty());
    assert(sts.get_value() == "move");
    std::string tt = "koishi";
    sts.emplace(tt.begin(), tt.end());
    assert(sts.get_value() == "koishi");

    // Test modifying
    sts.emplace_at("bs.ss", "bsss"s);
    assert(sts.size() == 1);
    assert(sts.get_value_optional("bs").has_value() == false);
    assert(sts.get_value("bs.ss") == "bsss");
    sts.modify("bs.ss", [](auto& in){ in += "-modified"; });
    assert(sts.get_value("bs.ss") == "bsss-modified");
    assert(sts.has_value("never.existed") == false);
    assert(sts.has_value("bs.ss"));

    string_tree<string> sts2;
    sts2.emplace_at("bs.ss" ,"not true");
    sts2.emplace_at("ss.ss", "true");

    sts.merge(sts2);
    assert(sts.get_value("ss.ss") == "true");
    assert(sts.get_value("bs.ss") != "not true");

    try
    {
        (void)sts.get_value("must.throw.exception");
        assert(false);
    }
    catch (const string_tree_base::path_not_found&) {}

    return 0;
}

