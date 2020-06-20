# Test UI elements

add_executable(
    node_test
    node_test.cpp
)
target_link_libraries(node_test PRIVATE "srose-test")
target_link_libraries(node_test PRIVATE "srose-ui")

add_test(
    NAME "TestNode"
    COMMAND node_test
)
