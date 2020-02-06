# Subterranean Rose i18n resource

macro(install_lang lang)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${lang} ${CMAKE_BINARY_DIR}/bin/locale/${lang}
    )
    install(
        DIRECTORY ${CMAKE_BINARY_DIR}/bin/locale
        DESTINATION bin
    )
endmacro(install_lang lang)

include(SRLC)
macro(compile_lang lang)
    file(GLOB_RECURSE files "${lang}/*.txt")
    compile_lc("${CMAKE_BINARY_DIR}/bin/locale/${lang}.srlc" ${files})
endmacro()

macro(test_lang lang)
    file(GLOB_RECURSE files "${lang}/*.txt")
    add_test(
	    NAME "TestLocale_${lang}"
	    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/lccl.py" "--action=error-checking" "-i" ${files}
    )
endmacro()


install_lang(en)
install_lang(zh-CN)
compile_lang(en)
compile_lang(zh-CN)
test_lang(en)
test_lang(zh-CN)
