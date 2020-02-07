/* Test parser of the Subterranean Rose i18n module */

#define BOOST_TEST_MODULE parser_test
#include <boost/test/included/unit_test.hpp>
#include <sr/locale/parser.hpp>
#include <string>
#include <sstream>


BOOST_AUTO_TEST_CASE(test_parser)
{
    using namespace srose;
    using namespace srose::locale;
    std::stringstream ss;
    ss.write("\x08\x00\x00\x00", 4);
    BOOST_REQUIRE(detail::Decode_U32LE(ss) == 0x8u);
    ss.write("\x04\x00\x00\x00""abcd", 8);
    BOOST_REQUIRE(detail::Decode_CxxStr(ss) == "abcd");
    // Test empty string
    ss.write("\x00\x00\x00\x00", 4);
    BOOST_REQUIRE(detail::Decode_CxxStr(ss) == std::string());
    // Test UTF-8
    ss.write("\x06\x00\x00\x00""\xE4\xB8\xAD\xE6\x96\x87", 10);
    BOOST_REQUIRE(detail::Decode_CxxStr(ss) == "中文");

    // Test string tree
    // The following data equal to
    // @fi = "first"
    // @fi.se = "second"
    unsigned char tree_data[39] =
    {
        0x02, 0x00, 0x00, 0x00, 0x66, 0x69, 0x05, 0x00,
        0x00, 0x00, 0x66, 0x69, 0x72, 0x73, 0x74, 0x01,
        0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x73,
        0x65, 0x06, 0x00, 0x00, 0x00, 0x73, 0x65, 0x63,
        0x6f, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x00,
    };
    ss.write((const char*)tree_data, 39);
    auto stree = ParseTranslation(ss);
    BOOST_REQUIRE(stree.get_value("fi") == "first");
    BOOST_REQUIRE(stree.get_value("fi.se") == "second");
}
