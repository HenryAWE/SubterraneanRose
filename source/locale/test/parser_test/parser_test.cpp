/* Test parser of the Subterranean Rose i18n module */

#include <sr/locale/parser.hpp>
#include <cassert>
#include <string>


int main()
{
    using namespace std;
    using namespace srose;
    using namespace srose::locale;

    util::string_tree<std::string> result;
    parse_content(result, "@name.text = \"Hello\"\" World\"");
    assert(result.get_value("name.text") == "Hello World");

    return 0;
}
