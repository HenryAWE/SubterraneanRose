/* Test language manager of the Subterranean Rose i18n module */

#define BOOST_TEST_MODULE language_test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/assertion.hpp>
#include <sr/locale/language.hpp>
#include <sr/util/semver_io.hpp>
#include <locale>
#include <boost/locale.hpp>


void test_stream_operator(std::shared_ptr<srose::locale::Language> lang);

BOOST_AUTO_TEST_CASE(test1)
{
    using namespace srose;
    using namespace srose::util;

    auto en = std::make_shared<locale::Language>("en.srlc");
    BOOST_TEST_REQUIRE(en->GetTextErrorAction() == locale::SRLC_RETURN_ERROR_STRING);
    BOOST_TEST_REQUIRE(en->GetId() == "en");
    BOOST_TEST_REQUIRE(en->GetName() == "English");
    BOOST_TEST_REQUIRE(en->GetVersion() == util::SemVer(0, 1, 0));
    BOOST_TEST_REQUIRE(en->GetAuthor() == "Subterranean Rose");
    BOOST_TEST_REQUIRE(en->GetComment() == "");
    BOOST_TEST_REQUIRE(en->GetTextOr("@not.found", "You'll get me") == "You'll get me");
    BOOST_TEST_REQUIRE(en->GetText("test.sub.hello") == "Greeting from sub-directory!");
    BOOST_TEST_REQUIRE(en->GetErrorString().has_value());
    BOOST_TEST_REQUIRE(*en->GetErrorString() == "Undefined");
    BOOST_TEST_REQUIRE(en->GetText("@not.found") == "Undefined");
    BOOST_TEST_REQUIRE(en->GetText("@not.found") == *en->GetErrorString());
    try
    {
        (void)en->GetTextWith("@not.found", locale::SRLC_THROW_EXCEPTION);
        BOOST_FAIL("Should not reach this line");
    }
    catch(const std::out_of_range&) {}

    boost::locale::generator gen;
    std::locale::global(gen("zh-CN.UTF-8"));
    auto zh_CN = std::make_shared<locale::Language>("zh-CN.srlc");
    BOOST_TEST_REQUIRE(zh_CN->GetTextErrorAction() == locale::SRLC_USE_FALLBACK);
    auto converted = boost::locale::conv::utf_to_utf<wchar_t>(zh_CN->GetText("srose.hello"));
    BOOST_TEST_REQUIRE(bool(converted == L"你好世界"));
    BOOST_TEST_REQUIRE(zh_CN->GetText("srose.hello") == "你好世界");
    BOOST_TEST_REQUIRE(zh_CN->GetName() == "简体中文");
    BOOST_TEST_REQUIRE(zh_CN->GetAuthor() == "Subterranean Rose");
    BOOST_TEST_REQUIRE(zh_CN->GetComment() == "这是注释");
    try
    {
        // Testing default string
        BOOST_TEST_REQUIRE(zh_CN->GetErrorString().has_value() == false);
        BOOST_TEST_REQUIRE(zh_CN->GetTextWith("@not.found", locale::SRLC_RETURN_ERROR_STRING) == "（加载失败）");
        BOOST_FAIL("Should not reach this line");
    }
    catch(std::invalid_argument) {}

    locale::LanguageSet lang_set;
    lang_set.insert(en);
    lang_set.insert(zh_CN);

    // Testing fallback
    BOOST_TEST_REQUIRE(zh_CN->GetFallbackId().has_value());
    BOOST_TEST_REQUIRE(*zh_CN->GetFallbackId() == "en");
    zh_CN->LinkFallback(lang_set);
    BOOST_TEST_REQUIRE(en->GetText("only.exist.in.English") == "This will only appear in English");
    BOOST_TEST_REQUIRE(zh_CN->GetTextWith("only.exist.in.English", locale::SRLC_USE_FALLBACK) == "This will only appear in English");
    BOOST_TEST_REQUIRE(zh_CN->GetText("only.exist.in.English") == "This will only appear in English");

    test_stream_operator(zh_CN);
}

void test_stream_operator(std::shared_ptr<srose::locale::Language> lang)
{
    using namespace srose;
    using namespace srose::locale;

    std::locale lang_lc(std::locale(), new TranslationFacet(lang));
    auto& tr = std::use_facet<TranslationFacet>(lang_lc).get();
    BOOST_TEST_REQUIRE(tr.GetName() == "简体中文");

    std::stringstream ss;
    ss.imbue(lang_lc);
    BOOST_TEST_REQUIRE(std::has_facet<TranslationFacet>(ss.getloc()));
    ss << SRTR("srose.hello");
    BOOST_TEST_REQUIRE(ss.str() == "你好世界");

    auto origin = std::locale::global(lang_lc);
    BOOST_TEST_REQUIRE(!std::has_facet<TranslationFacet>(origin));
    BOOST_TEST_REQUIRE(SRTR("srose.hello").str() == "你好世界");
    std::string use_fallback = SRTR("@use.fallback");
    BOOST_TEST_REQUIRE(use_fallback == "Undefined");

    std::locale::global(CreateTranslation(origin, lang));
    BOOST_TEST_REQUIRE(SRTR("srose.hello").str() == "你好世界");
    std::string use_fallback_2 = SRTR("@use.fallback");
    BOOST_TEST_REQUIRE(use_fallback_2 == "Undefined");
}

BOOST_AUTO_TEST_CASE(test_circular_dependency_checking)
{
    using namespace srose;
    using namespace srose::locale;

    auto a = std::make_shared<locale::Language>("a.srlc");
    auto b = std::make_shared<locale::Language>("b.srlc");
    auto c = std::make_shared<locale::Language>("c.srlc");

    BOOST_TEST(a->GetAuthor() == "Unknown");
    BOOST_TEST(b->GetAuthor() == "Unknown");
    BOOST_TEST(c->GetAuthor() == "Unknown");

    BOOST_TEST(a->GetComment() == "Hello world");
    BOOST_TEST(b->GetComment() == "Hello world");
    BOOST_TEST(c->GetComment() == "Hello world");

    BOOST_TEST(a->GetTextWith("dummy.text", locale::SRLC_THROW_EXCEPTION) == "dummy");
    BOOST_TEST(b->GetTextWith("dummy.text", locale::SRLC_THROW_EXCEPTION) == "dummy");
    BOOST_TEST(c->GetTextWith("dummy.text", locale::SRLC_THROW_EXCEPTION) == "dummy");

    locale::LanguageSet lang_set;
    lang_set.insert(a);
    lang_set.insert(b);
    lang_set.insert(c);

    bool reached = false;
    try
    {
        a->LinkFallback(lang_set);
        b->LinkFallback(lang_set);
        reached = true;
        c->LinkFallback(lang_set);
        BOOST_FAIL("Unreachable");
    }
    catch(const std::logic_error&) {}
    BOOST_TEST(reached == true);
}
