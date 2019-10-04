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

    return 0;
}
