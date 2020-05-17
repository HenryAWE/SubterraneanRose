# Test virtual filesystem of resource module

add_executable(
    vfs_test
    vfs_test.cpp
)
target_link_libraries(vfs_test PRIVATE "srose-test")
target_link_libraries(vfs_test PRIVATE "srose-res")

add_test(
    NAME "TestVfs"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/vfs_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
