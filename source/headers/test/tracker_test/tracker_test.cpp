/* Test variable tracker */

#define BOOST_TEST_MODULE tracker_test
#include <boost/test/included/unit_test.hpp>
#include <sr/util/tracker.hpp>
#include <string>


BOOST_AUTO_TEST_CASE(test1)
{
    using namespace srose::util;
    tracked<int> tr(in_place, 2333);
    BOOST_TEST_REQUIRE(tr.dirty());
    BOOST_TEST_REQUIRE(tr.value() == 2333);
    tracked<int> copied_tr = tr;
    BOOST_TEST_REQUIRE(copied_tr.value() == 2333);
    BOOST_TEST_REQUIRE(copied_tr.dirty());

    tracked<int> init_clean(init_as_clean, 666);
    BOOST_TEST_REQUIRE(init_clean.value() == 666);
    BOOST_TEST_REQUIRE(init_clean.clean());
    BOOST_TEST_REQUIRE(!init_clean.dirty());
    init_clean.mark_as_dirty();
    BOOST_TEST_REQUIRE(init_clean.dirty());
    init_clean.mark_as_clean();
    BOOST_TEST_REQUIRE(init_clean.clean());

    init_clean.assign(555);
    BOOST_TEST_REQUIRE(init_clean.dirty());
    BOOST_TEST_REQUIRE(init_clean.value() == 555);

    std::string moved_str = "will be moved";
    tracked<std::string> tr_str(in_place, "original");
    BOOST_TEST_REQUIRE(tr_str.value() == "original");
    tr_str.assign(std::move(moved_str));
    BOOST_TEST_REQUIRE(moved_str.empty());
    BOOST_TEST_REQUIRE(tr_str.value() == "will be moved");

    std::string moved_str_2 = "will be moved too";
    tracked<std::string> tr_str_2(in_place, std::move(moved_str_2));
    BOOST_TEST_REQUIRE(tr_str_2.dirty());
    BOOST_TEST_REQUIRE(moved_str_2.empty());
    BOOST_TEST_REQUIRE(tr_str_2.value() == "will be moved too");

    std::string moved_str_3 = "third one";
    tracked<std::string> tr_str_3(init_as_clean, std::move(moved_str_3));
    BOOST_TEST_REQUIRE(tr_str_3.clean());
    BOOST_TEST_REQUIRE(moved_str_3.empty());
    BOOST_TEST_REQUIRE(tr_str_3.value() == "third one");
}
