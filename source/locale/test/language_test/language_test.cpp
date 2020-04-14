/* Test language manager of the Subterranean Rose i18n module */

#define BOOST_TEST_MODULE language_test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/assertion.hpp>
#include <sr/locale/language.hpp>
#include <locale>
#include <boost/locale.hpp>


void test_stream_operator(std::shared_ptr<srose::locale::Language> lang);

BOOST_AUTO_TEST_CASE(test1)
{
    using namespace srose;
    using namespace srose::util;

    locale::Language default_lc{};
    BOOST_TEST_REQUIRE(default_lc.gettext("srose.language.name") == "Default");
    BOOST_TEST_REQUIRE(default_lc.gettext("srose.language.iso") == "C");

    locale::Language en("en.srlc");
    BOOST_TEST_REQUIRE(en.gettext("srose.language.name") == "English");
    BOOST_TEST_REQUIRE(en.name() == "English");
    BOOST_TEST_REQUIRE(en.iso() == "en");
    BOOST_TEST_REQUIRE(en.gettext("srose.language.name", "ERROR") == "English");
    BOOST_TEST_REQUIRE(en.gettext("@not.found", "You'll get me") == "You'll get me");
    BOOST_TEST_REQUIRE(en.gettext("test.sub.hello") == "Greeting from sub-directory!");
    BOOST_TEST_REQUIRE(en.default_str().has_value() == false);
    try
    {
        (void)en.gettext("@not.found");
        BOOST_FAIL("Should not reach this line");
    }
    catch(const string_tree_base::path_not_found&) {}

    boost::locale::generator gen;
    std::locale::global(gen("zh-CN.UTF-8"));
    locale::Language zh_CN("zh-CN.srlc");
    auto converted = boost::locale::conv::utf_to_utf<wchar_t>(zh_CN.gettext("srose.language.name"));
    BOOST_TEST_REQUIRE(bool(converted == L"简体中文"));
    BOOST_TEST_REQUIRE(zh_CN.gettext("srose.language.name") == "简体中文");
    BOOST_TEST_REQUIRE(zh_CN.name() == "简体中文");
    BOOST_TEST_REQUIRE(zh_CN.gettext("srose.language.default") == "（加载失败）");
    BOOST_TEST_REQUIRE(zh_CN.default_str().has_value());
    BOOST_TEST_REQUIRE(bool(*zh_CN.default_str() == "（加载失败）"));
    try
    {
        // Testing default string
        BOOST_TEST_REQUIRE(zh_CN.gettext("@not.found") == "（加载失败）");
    }
    catch(const string_tree_base::path_not_found&)
    {
        BOOST_FAIL("Should not reach this line");
    }

    // Testing fallback
    zh_CN.fallback(std::make_shared<locale::Language>(en));
    BOOST_TEST_REQUIRE(en.gettext("only.exist.in.English") == "This will only appear in English");
    BOOST_TEST_REQUIRE(zh_CN.gettext("only.exist.in.English", zh_CN.use_fallback) == "This will only appear in English");
    BOOST_TEST_REQUIRE(zh_CN.gettext("@not.found", zh_CN.use_fallback) == *zh_CN.default_str());

    test_stream_operator(std::make_shared<locale::Language>(std::move(zh_CN)));
}

void test_stream_operator(std::shared_ptr<srose::locale::Language> lang)
{
    BOOST_TEST_REQUIRE(lang->default_str().has_value());
    BOOST_TEST_REQUIRE(bool(*lang->default_str() == "（加载失败）"));

    using namespace srose;
    using namespace srose::locale;
    std::locale lang_lc(std::locale(), new TranslationFacet(lang));
    auto& tr = std::use_facet<TranslationFacet>(lang_lc).get();
    BOOST_TEST_REQUIRE(tr.name() == "简体中文");

    std::stringstream ss;
    ss.imbue(lang_lc);
    BOOST_TEST_REQUIRE(std::has_facet<TranslationFacet>(ss.getloc()));
    ss << SRTR("srose.language.name");
    BOOST_TEST_REQUIRE(ss.str() == "简体中文");

    auto origin = std::locale::global(lang_lc);
    BOOST_TEST_REQUIRE(!std::has_facet<TranslationFacet>(origin));
    BOOST_TEST_REQUIRE(SRTR("srose.language.name").str() == "简体中文");
    std::string not_found = SRTR("@not.found");
    BOOST_TEST_REQUIRE(not_found == *lang->default_str());

    std::locale::global(CreateTranslation(origin, lang));
    BOOST_TEST_REQUIRE(SRTR("srose.language.name").str() == "简体中文");
    std::string not_found_2 = SRTR("@not.found");
    BOOST_TEST_REQUIRE(not_found_2 == *lang->default_str());
}
