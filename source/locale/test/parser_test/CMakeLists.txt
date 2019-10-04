# Test parser of i18n module

add_executable(
    parser_test
    parser_test.cpp
)
target_link_libraries(parser_test PRIVATE "srose-locale")
target_link_libraries(parser_test PRIVATE Boost::locale)

add_test(
    NAME "TestParser"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/parser_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
