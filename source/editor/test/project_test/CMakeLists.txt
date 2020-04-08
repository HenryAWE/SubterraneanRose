# Test editor project

add_executable(
    project_test
    project_test.cpp
)
target_link_libraries(project_test PRIVATE "srose-test")
target_link_libraries(project_test PRIVATE "srose-editor")

add_test(
    NAME "TestProject"
    COMMAND project_test
)
