/**
 * @file profile_test.cpp
 * @author your name (you@domain.com)
 * @brief Test user profile
 */

#define BOOST_TEST_MODULE profile_test
#include <boost/test/included/unit_test.hpp>
#include <sr/srose/user.hpp>
#include <sstream>
#include <set>


BOOST_AUTO_TEST_CASE(test_profile_save_load)
{
    using namespace srose;
    UserProfile p1("user1");
    p1.Set("data.1", "data1");

    std::stringstream ss;
    p1.Save(ss);

    UserProfile p2(ss);
    BOOST_TEST_REQUIRE(p2.Get("data.1") == "data1");
    p2.name("user2");

    std::stringstream ss2;
    p1.Save(ss2);
    p2.Save(ss2);

    typedef std::set<UserProfile, UserProfile::Comparator> Users;
    Users users;
    while(ss2)
    {
        users.emplace(ss2);
    }
    BOOST_TEST_REQUIRE(users.count("user1") == 1);
    BOOST_TEST_REQUIRE(users.count("user2") == 1);
    BOOST_TEST_REQUIRE(users.count("user3") == 0);
}

BOOST_AUTO_TEST_CASE(test_profile)
{
    using namespace srose;

    UserProfile p1("name");
    p1.Set("data.1", "data1");
    p1.Set("data.2", "data2");
    BOOST_TEST_REQUIRE(p1.name() == "name");
    BOOST_TEST_REQUIRE(p1.Get("data.1") == "data1");
    BOOST_TEST_REQUIRE(p1.Get("data.2") == "data2");
    p1.name("new name");
    BOOST_TEST_REQUIRE(p1.name() == "new name");
}
