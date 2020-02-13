# Subterranean Rose i18n resource

include(SRLC)
macro(compile_lang lang)
    file(GLOB_RECURSE files "${lang}/*.txt")
    compile_lc("${CMAKE_BINARY_DIR}/bin/locale/${lang}.srlc" ${files})
    install(
        FILES ${CMAKE_BINARY_DIR}/bin/locale/${lang}.srlc
        DESTINATION bin/locale
    )
endmacro()

macro(test_lang lang)
    file(GLOB_RECURSE files "${lang}/*.txt")
    add_test(
	    NAME "TestLocale_${lang}"
	    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/lccl.py" "--action=error-checking" "-i" ${files}
    )
endmacro()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/locale)
compile_lang(en)
compile_lang(zh-CN)
test_lang(en)
test_lang(zh-CN)
