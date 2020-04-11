/**
 * @file profile_test.cpp
 * @author HenryAWE
 * @brief Test editor project
 */

#define BOOST_TEST_MODULE project_test
#include <boost/test/included/unit_test.hpp>
#include <sr/editor/project.hpp>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


BOOST_AUTO_TEST_CASE(test_project_serialization)
{
    using namespace srose;

    std::stringstream ss;

    {
        boost::archive::text_oarchive oa(ss);
        editor::Project proj;
        proj.name("test serialization");
        oa << proj;
    }

    {
        boost::archive::text_iarchive ia(ss);
        editor::Project proj;
        ia >> proj;
        BOOST_TEST_REQUIRE(proj.name() == "test serialization");
    }
}

BOOST_AUTO_TEST_CASE(test_project_properties)
{
    using namespace srose;
    editor::Project proj;
    BOOST_TEST_REQUIRE(proj.name() == "Subterranean Rose Editor Project");
    proj.name("test");
    BOOST_TEST_REQUIRE(proj.name() == "test");
}

BOOST_AUTO_TEST_CASE(test_project_function)
{
    using namespace srose;
    editor::Project proj;
    proj.name("original");
    BOOST_TEST_REQUIRE(proj.Duplicate().name() == "original.1");
    proj.name("original");
    BOOST_TEST_REQUIRE(proj.Duplicate(false).name() == "original");
    proj.name("original.2");
    BOOST_TEST_REQUIRE(proj.Duplicate().name() == "original.3");
    proj.name("original.4");
    BOOST_TEST_REQUIRE(proj.Duplicate(false).name() == "original.4");
}
