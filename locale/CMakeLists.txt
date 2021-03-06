# Subterranean Rose i18n resource

include(SRLC)
macro(compile_lang lang)
    file(GLOB_RECURSE files "${lang}/*.txt")
    compile_lc("${CMAKE_BINARY_DIR}/bin/locale/${lang}.srlc" "${CMAKE_CURRENT_SOURCE_DIR}/${lang}/${lang}.xml" ${files})
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
compile_lang(en-US)
compile_lang(zh-CN)
if(NOT ${SROSE_DISABLE_TEST})
    test_lang(en-US)
    test_lang(zh-CN)
endif()

include(SRf2c)
file(GLOB_RECURSE C_files "C/*.txt")
compile_lc("${CMAKE_BINARY_DIR}/embedded_C.srlc" "${CMAKE_CURRENT_SOURCE_DIR}/C/C.xml" ${C_files})
file2cstr("${CMAKE_BINARY_DIR}/embedded_C.srlc" "${CMAKE_BINARY_DIR}/embedded_C.inl")
