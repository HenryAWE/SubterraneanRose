# Test user profile

add_executable(
    profile_test
    profile_test.cpp
)
target_link_libraries(profile_test PRIVATE "srose-test")
target_link_libraries(profile_test PRIVATE "srose-main")

add_test(
    NAME "TestProfile"
    COMMAND profile_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
