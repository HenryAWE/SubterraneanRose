# Test semantic version library

add_executable(
    semver_test
    semver_test.cpp
)
target_link_libraries(semver_test PRIVATE "srose-test")
target_link_libraries(semver_test PRIVATE "srose-headers")

add_test(
    NAME "TestSemver"
    COMMAND semver_test
)
