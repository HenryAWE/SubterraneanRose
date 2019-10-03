# Test StringTree
add_executable(
    string_tree_test
    string_tree_test.cpp
)
target_link_libraries(string_tree_test PRIVATE "srose-util-headers")

add_test(
    NAME "TestStringTree"
    COMMAND string_tree_test
)
