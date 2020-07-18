/**
 * @file semver_test.cpp
 * @author HenryAWE
 * @brief Test semantic version library
 */

#define BOOST_TEST_MODULE semver_test
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <sr/util/semver.hpp>
#include <sr/util/semver_io.hpp>


BOOST_AUTO_TEST_CASE(test_semver)
{
    using srose::util::SemVer;

    SemVer zero;
    BOOST_TEST(zero.major() == 0);
    BOOST_TEST(zero.minor() == 0);
    BOOST_TEST(zero.patch() == 0);

    BOOST_TEST(zero == SemVer(0, 0, 0));
    BOOST_TEST(zero <= SemVer(0, 0, 0));
    BOOST_TEST(zero >= SemVer(0, 0, 0));
    BOOST_TEST(zero != SemVer(1, 0, 0));
    BOOST_TEST(zero < SemVer(0, 0, 1));
    BOOST_TEST(zero < SemVer(0, 1, 1));
    BOOST_TEST(zero < SemVer(1, 1, 1));
    BOOST_TEST(zero < SemVer(1, 1, 0));
    BOOST_TEST(zero < SemVer(1, 0, 1));
    BOOST_TEST(zero < SemVer(1, 0, 0));

    SemVer ver;
    ver = SemVer(1, 0, 0);
    BOOST_TEST(ver.major() == 1);
    BOOST_TEST(ver.minor() == 0);
    ver.minor() = 1;
    BOOST_TEST(ver.minor() == 1);
}

BOOST_AUTO_TEST_CASE(test_semver_io)
{
    using srose::util::SemVer;

    SemVer semver(0, 1, 0);
    std::stringstream ss;

    ss << semver;
    BOOST_TEST(ss.str() == "0.1.0");
}
