# Test function queue

add_executable(
    funcqueue_test
    funcqueue_test.cpp
)
target_link_libraries(funcqueue_test PRIVATE "srose-test")
target_link_libraries(funcqueue_test PRIVATE "srose-headers")

add_test(
    NAME "TestFuncQueue"
    COMMAND funcqueue_test
)
