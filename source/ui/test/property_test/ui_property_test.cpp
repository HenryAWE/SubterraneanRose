/* Test UI property */
#include <sr/ui/property.hpp>
#include <cassert>
#include <climits>
#include <cfloat>
#include <cmath>


using namespace srose::ui;

template <typename T> 
void test_normal()
{
    Property<T> pt("T test");
    assert(pt.name() == "T test");
    assert(pt.value() == T());
    assert(pt.min() == std::numeric_limits<T>::min());
    assert(pt.max() == std::numeric_limits<T>::max());
}

int main()
{
    Property<int> pi("test", 0, -15);
    assert(pi.value() == 0);
    assert(pi.min() == -15);
    assert(pi.max() == INT_MAX);
    pi.value(-16); // Will be clamped
    assert(pi.value() == -15);
    assert(pi.min() == -15);
    pi.min(INT_MIN);
    assert(pi.min() == INT_MIN);
    pi.value(-100);
    assert(pi.value() == -100);

    Property<float> pf("float", 15.0f);
    assert(std::abs(pf.value() - 15.0f) < FLT_EPSILON);

    test_normal<char>();
    test_normal<char16_t>();
    test_normal<wchar_t>();
    test_normal<char32_t>();
    test_normal<unsigned long>();

    Property<std::string> ts("string", "init");
    assert(ts.name() == "string");
    assert(ts.value() == "init");
    ts.value("modified");
    assert(ts.value() == "modified");

    return 0;
}
