/* Test UI property */

#define BOOST_TEST_MODULE ui_property_test
#include <boost/test/included/unit_test.hpp>
#include <sr/ui/property.hpp>
#include <climits>
#include <cfloat>
#include <cmath>


using namespace srose::ui;

template <typename T> 
void test_normal()
{
    Property<T> pt("T test");
    BOOST_REQUIRE(pt.name() == "T test");
    BOOST_REQUIRE(pt.value() == T());
    BOOST_REQUIRE(pt.min() == std::numeric_limits<T>::min());
    BOOST_REQUIRE(pt.max() == std::numeric_limits<T>::max());
}

BOOST_AUTO_TEST_CASE(test1)
{
    Property<int> pi("test", 0, -15);
    BOOST_REQUIRE(pi.value() == 0);
    BOOST_REQUIRE(pi.min() == -15);
    BOOST_REQUIRE(pi.max() == INT_MAX);
    pi.value(-16); // Will be clamped
    BOOST_REQUIRE(pi.value() == -15);
    BOOST_REQUIRE(pi.min() == -15);
    pi.min(INT_MIN);
    BOOST_REQUIRE(pi.min() == INT_MIN);
    pi.value(-100);
    BOOST_REQUIRE(pi.value() == -100);

    Property<float> pf("float", 15.0f);
    BOOST_REQUIRE(std::abs(pf.value() - 15.0f) < FLT_EPSILON);

    test_normal<char>();
    test_normal<char16_t>();
    test_normal<wchar_t>();
    test_normal<char32_t>();
    test_normal<unsigned long>();

    Property<std::string> ts("string", "init");
    BOOST_REQUIRE(ts.name() == "string");
    BOOST_REQUIRE(ts.value() == "init");
    ts.value("modified");
    BOOST_REQUIRE(ts.value() == "modified");
}
