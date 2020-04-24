# Test variable tracker

add_executable(
    tracker_test
    tracker_test.cpp
)
target_link_libraries(tracker_test PRIVATE "srose-test")
target_link_libraries(tracker_test PRIVATE "srose-headers")

add_test(
    NAME "TestTracker"
    COMMAND tracker_test
)
