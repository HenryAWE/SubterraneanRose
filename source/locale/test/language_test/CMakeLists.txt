# Test language manager of i18n module

add_executable(
    language_test
    language_test.cpp
)
target_link_libraries(language_test PRIVATE "srose-test")
target_link_libraries(language_test PRIVATE "srose-locale")
target_link_libraries(language_test PRIVATE Boost::locale)

add_test(
    NAME "TestLanguage"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/language_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
