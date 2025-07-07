# Simple dependency verification test
# This tests that all our dependencies are properly installed and accessible

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Test)
find_package(OpenCV REQUIRED)

# Create a simple dependency test
qt6_add_executable(test_dependencies test_dependencies.cpp)

target_link_libraries(test_dependencies PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Test
    ${OpenCV_LIBS}
)

target_include_directories(test_dependencies PRIVATE ${OpenCV_INCLUDE_DIRS})

# Add the test
add_test(NAME DependencyTest COMMAND test_dependencies)
