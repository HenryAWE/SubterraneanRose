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
	    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/lccl.py" "--action=check" "-i" ${files}
    )
endmacro()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/locale)
compile_lang(zh-CN)
test_lang(en)
test_lang(zh-CN)

include(SRf2c)
file(GLOB_RECURSE en_files "en/*.txt")
compile_lc("${CMAKE_BINARY_DIR}/embedded_en.srlc" ${en_files})
file2cstr("${CMAKE_BINARY_DIR}/embedded_en.srlc" "${CMAKE_BINARY_DIR}/embedded_en.inl")
