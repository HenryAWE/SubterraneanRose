/* Test parser of the Subterranean Rose i18n module */

#define BOOST_TEST_MODULE parser_test
#include <boost/test/included/unit_test.hpp>
#include <sr/locale/parser.hpp>
#include <string>
#include <sstream>
#include <boost/locale/encoding_utf.hpp>


const char translations[] =
R"(
# This is a comment
@srose.first = "First Message"
@srose.second = "第二条信息"
)";

const wchar_t wtranslations[] =
LR"(
# 这是一条注释
@text.zh = "中文"
@text.ru = "Россия"
@text.ja = "にほんご"
)";


BOOST_AUTO_TEST_CASE(test1)
{
    using srose::locale::parse_string, srose::locale::parse_stream, srose::locale::parse_wstream;

    srose::util::string_tree<std::string> result;
    parse_string(result, L"@name.text = \"Hello\"\" World\"");
    BOOST_TEST_REQUIRE(result.get_value("name.text") == "Hello World");

    std::stringstream ss;
    ss.str(translations);
    auto result2 = parse_stream(ss);
    BOOST_TEST_REQUIRE(result2.get_value("srose.first") == "First Message");
    auto converted = boost::locale::conv::utf_to_utf<wchar_t>(result2.get_value("srose.second"));
    BOOST_TEST_REQUIRE(bool(converted == L"第二条信息"));
    BOOST_TEST_REQUIRE(result2.get_value("srose.second") == "第二条信息");

    std::wstringstream wss;
    wss.str(wtranslations);
    auto result3 = parse_wstream(wss);
    BOOST_TEST_REQUIRE(result3.get_value("text.zh") == "中文");
    BOOST_TEST_REQUIRE(result3.get_value("text.ru") == "Россия");
    BOOST_TEST_REQUIRE(result3.get_value("text.ja") == "にほんご");
}
