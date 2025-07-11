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
    Test
)

# Find other dependencies
find_package(OpenCV REQUIRED)

# Qt6 automatic MOC, UIC, and RCC
qt6_standard_project_setup()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${OpenCV_INCLUDE_DIRS})

# Main application source files
set(SOURCES
    src/main.cpp
    src/OpenGL3DViewport.cpp
)

set(HEADERS
    src/OpenGL3DViewport.hpp
)

# QML files
set(QML_FILES
    src/qml/main.qml
    src/qml/MainWindow.qml
    src/qml/components/ModeSelector.qml
    src/qml/components/ShapeSelector.qml
    src/qml/components/InputDeviceSelector.qml
    src/qml/components/SessionControls.qml
    src/qml/components/TransformDisplay.qml
    src/qml/components/Viewport3D.qml
)

# Create the main executable
qt6_add_executable(ManualRegistrationGL_V2
    ${SOURCES}
    ${HEADERS}
)

# Add QML module
qt6_add_qml_module(ManualRegistrationGL_V2
    URI SURGAR.Components
    VERSION 1.0
    QML_FILES ${QML_FILES}
    SOURCES ${SOURCES} ${HEADERS}
)

# Link libraries for main application
target_link_libraries(ManualRegistrationGL_V2 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::OpenGL
    ${OpenCV_LIBS}
)

# Include OpenCV headers
target_include_directories(ManualRegistrationGL_V2 PRIVATE ${OpenCV_INCLUDE_DIRS})

# Set output directory
set_target_properties(ManualRegistrationGL_V2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Enable testing (optional)
if(BUILD_TESTING)
    enable_testing()

    # Create test executable
    qt6_add_executable(test_dependencies
        tests/test_dependencies.cpp
    )

    target_link_libraries(test_dependencies PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::OpenGL
        Qt6::Test
        ${OpenCV_LIBS}
    )

    add_test(NAME DependencyTest COMMAND test_dependencies)
endif()

# Installation rules
install(TARGETS ManualRegistrationGL_V2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Copy QML files for development
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Create symlinks or copy QML files to build directory for development
    file(COPY ${CMAKE_SOURCE_DIR}/src/qml DESTINATION ${CMAKE_BINARY_DIR}/src)
endif()