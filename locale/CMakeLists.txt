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


install_lang(en)
install_lang(zh-CN)
