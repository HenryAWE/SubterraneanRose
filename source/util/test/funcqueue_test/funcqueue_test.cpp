/*Test function queue */

#define BOOST_TEST_MODULE funcqueue_test
#include <boost/test/included/unit_test.hpp>
#include <sr/util/funcqueue.hpp>
#include <string>

using namespace std;
using namespace srose::util;

template <typename T, typename U>
struct setter
{
    T& target;
    U value;

    setter(T& target_, U value_)
        : target(target_), value(move(value_)) {}

    void operator()()
    {
        target = value;
    }
};

BOOST_AUTO_TEST_CASE(test_funcqueue)
{
    funcqueue fq;
    string s = "begin";
    BOOST_TEST_REQUIRE(bool(s == "begin"));
    fq.then(setter(s, "new"));
    fq.invoke();
    BOOST_TEST_REQUIRE(bool(s == "new"));
    int var = 0;
    fq.repeat(5, [&var](){ ++var; });
    BOOST_TEST_REQUIRE(var == 0);
    for(int i = 0; i<5; ++i)
    {
        fq.invoke();
        BOOST_TEST_REQUIRE(var = i+1);
    }
}
