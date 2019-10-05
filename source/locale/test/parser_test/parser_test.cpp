/* Test parser of the Subterranean Rose i18n module */

#include <sr/locale/parser.hpp>
#include <cassert>
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


int main()
{
    using namespace std;
    using namespace srose;
    using namespace srose::locale;

    util::string_tree<std::string> result;
    parse_string(result, L"@name.text = \"Hello\"\" World\"");
    assert(result.get_value("name.text") == "Hello World");

    std::stringstream ss;
    ss.str(translations);
    auto result2 = parse_stream(ss);
    assert(result2.get_value("srose.first") == "First Message");
    assert(boost::locale::conv::utf_to_utf<wchar_t>(result2.get_value("srose.second")) == L"第二条信息");
    assert(result2.get_value("srose.second") == "第二条信息");

    std::wstringstream wss;
    wss.str(wtranslations);
    auto result3 = parse_wstream(wss);
    assert(result3.get_value("text.zh") == "中文");
    assert(result3.get_value("text.ru") == "Россия");
    assert(result3.get_value("text.ja") == "にほんご");

    return 0;
}
