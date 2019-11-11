# Test UI property

add_executable(
    ui_property_test
    ui_property_test.cpp
)
target_link_libraries(ui_property_test PRIVATE "srose-ui")

add_test(
    NAME "TestUIProperty"
    COMMAND ui_property_test
)
