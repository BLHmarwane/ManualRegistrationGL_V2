cmake_minimum_required(VERSION 3.16)
project(ManualRegistrationGL_V2 VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    OpenGL
)

# Find other dependencies
find_package(OpenCV REQUIRED)

# Qt6 automatic MOC, UIC, and RCC
qt6_standard_project_setup()

# Create the main executable first
qt6_add_executable(ManualRegistrationGL_V2
    src/main.cpp
)

# Add QML resources to the executable
qt6_add_resources(ManualRegistrationGL_V2 "qml"
    PREFIX "/"
    FILES
        src/qml/main.qml
        src/qml/MainWindow.qml
        src/qml/components/Viewport3D.qml
        src/qml/components/ModeSelector.qml
        src/qml/components/ShapeSelector.qml
        src/qml/components/SessionControls.qml
        src/qml/components/TransformDisplay.qml
)

# Include directories
target_include_directories(ManualRegistrationGL_V2 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(ManualRegistrationGL_V2 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::OpenGL
    ${OpenCV_LIBS}
)

# Set output directory
set_target_properties(ManualRegistrationGL_V2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Enable testing
enable_testing()
add_subdirectory(tests)