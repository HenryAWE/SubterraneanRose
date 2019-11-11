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
    Property<T> pt("test");
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

    Property<float> pf("float", 15.0f);
    assert(std::abs(pf.value() - 15.0f) < FLT_EPSILON);

    test_normal<char>();
    test_normal<wchar_t>();

    return 0;
}
