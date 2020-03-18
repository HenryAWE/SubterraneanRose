# Test ECS framework
add_executable(
    ecs_test
    ecs_test.cpp
)
target_link_libraries(ecs_test PRIVATE "srose-test")
target_link_libraries(ecs_test PRIVATE "srose-player")

add_test(
    NAME "TestECS"
    COMMAND ecs_test
)
