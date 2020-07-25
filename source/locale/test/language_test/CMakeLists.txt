# Test language manager of i18n module

include(SRLC)

add_executable(
    language_test
    language_test.cpp
)
target_link_libraries(language_test PRIVATE "srose-test")
target_link_libraries(language_test PRIVATE "srose-locale")
target_link_libraries(language_test PRIVATE Boost::locale)
compile_lc("${CMAKE_CURRENT_BINARY_DIR}/en.srlc" "${CMAKE_CURRENT_SOURCE_DIR}/locale/en/en.xml" "${CMAKE_CURRENT_SOURCE_DIR}/locale/en/test.txt" "${CMAKE_CURRENT_SOURCE_DIR}/locale/en/sub/test-sub.txt")
compile_lc("${CMAKE_CURRENT_BINARY_DIR}/zh-CN.srlc" "${CMAKE_CURRENT_SOURCE_DIR}/locale/zh-CN/zh-CN.xml" "${CMAKE_CURRENT_SOURCE_DIR}/locale/zh-CN/test.zh-CN.txt")

add_test(
    NAME "TestLanguage"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/language_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
