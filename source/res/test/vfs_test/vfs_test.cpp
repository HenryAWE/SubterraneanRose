/**
 * @file vfs_test.cpp
 * @author HenryAWE
 * @brief Test virtual filesystem
 */

#define BOOST_TEST_MODULE vfs_test
#include <boost/test/included/unit_test.hpp>
#include <sr/res/vfs.hpp>


BOOST_AUTO_TEST_CASE(test_vfs_path)
{
    using namespace srose::vfs;
    Path pt = "/data/path";
    std::vector<std::string> expected = { "data", "path" };

    int index = 0;
    for(auto& i : pt)
    {
        BOOST_TEST(i == expected.at(index++));
    }

    BOOST_TEST(*pt.Parent().begin() == "data");
    BOOST_TEST(pt.Parent().Parent().string() == "/");
    BOOST_TEST(pt.Parent().Parent().Parent().string() == "/");

    BOOST_TEST(Path("/data/data.txt").Filename() == "data.txt");
}

BOOST_AUTO_TEST_CASE(test_mounting)
{
    using namespace srose::vfs;
    VirtualFilesystem vfs;

    vfs.MountNativeFile("/data/test.txt", "test.txt");

    auto stream = vfs.Open("/data/test.txt");
    std::string content;
    std::getline(*stream, content);
    BOOST_TEST(content == "Testing virtual filesystem");
}
