# Test String Algorithm
add_executable(
    stralgo_test
    stralgo_test.cpp
)
target_link_libraries(stralgo_test PRIVATE "srose-test")
target_link_libraries(stralgo_test PRIVATE "srose-srtl")

add_test(
    NAME "TestStringAlgorithm"
    COMMAND stralgo_test
)
