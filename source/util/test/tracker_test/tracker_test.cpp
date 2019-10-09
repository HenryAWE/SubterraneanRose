/* Test variable tracker */

#include <sr/util/tracker.hpp>
#include <cassert>
#include <string>


int main()
{
    using namespace srose::util;
    tracked<int> tr(in_place, 2333);
    assert(tr.dirty());
    assert(tr.value() == 2333);
    tracked<int> copied_tr = tr;
    assert(copied_tr.value() == 2333);
    assert(copied_tr.dirty());

    tracked<int> init_clean(init_as_clean, 666);
    assert(init_clean.value() == 666);
    assert(init_clean.clean());
    assert(!init_clean.dirty());
    init_clean.mark_as_dirty();
    assert(init_clean.dirty());
    init_clean.mark_as_clean();
    assert(init_clean.clean());

    init_clean.assign(555);
    assert(init_clean.dirty());
    assert(init_clean.value() == 555);

    std::string moved_str = "will be moved";
    tracked<std::string> tr_str(in_place, "original");
    assert(tr_str.value() == "original");
    tr_str.assign(std::move(moved_str));
    assert(moved_str.empty());
    assert(tr_str.value() == "will be moved");

    std::string moved_str_2 = "will be moved too";
    tracked<std::string> tr_str_2(in_place, std::move(moved_str_2));
    assert(tr_str_2.dirty());
    assert(moved_str_2.empty());
    assert(tr_str_2.value() == "will be moved too");

    std::string moved_str_3 = "third one";
    tracked<std::string> tr_str_3(init_as_clean, std::move(moved_str_3));
    assert(tr_str_3.clean());
    assert(moved_str_3.empty());
    assert(tr_str_3.value() == "third one");

    return 0;
}
