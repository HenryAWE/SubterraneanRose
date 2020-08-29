/**
 * @file stralgo_test.cpp
 * @author HenryAWE
 * @brief Testing string algorithm
 */

#define BOOST_TEST_MODULE stralgo_test
#include <boost/test/included/unit_test.hpp>
#include <srtl/stralgo.hpp>
#include <sstream>
#include <algorithm>


BOOST_AUTO_TEST_CASE(test_ostream_joiner)
{
    using namespace srtl;
    std::stringstream ss;
    int arr[] = { 1, 2, 3, 4, 5 };
    std::copy(
        std::begin(arr),
        std::end(arr),
        make_ostream_joiner(ss, ", ")
    );
    BOOST_TEST(ss.str() == "1, 2, 3, 4, 5");
}
