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

# Find OpenCV
find_package(OpenCV REQUIRED)

# Find HIDAPI for SpaceMouse support
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)

# Qt6 automatic MOC, UIC, and RCC
qt6_standard_project_setup()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${HIDAPI_INCLUDE_DIRS})

# Main application source files - NOW INCLUDING SpaceMouse
set(SOURCES
    src/main.cpp
    src/OpenGL3DViewport.cpp
    src/SpaceMouseManager.cpp
)

set(HEADERS
    src/OpenGL3DViewport.hpp
    src/SpaceMouseManager.hpp
)

# QML files - Only include files that actually exist
set(QML_FILES
    src/qml/main.qml
    src/qml/MainWindow.qml
)

# Check if component files exist and add them conditionally
if(EXISTS "${CMAKE_SOURCE_DIR}/src/qml/components/ModeSelector.qml")
    list(APPEND QML_FILES src/qml/components/ModeSelector.qml)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/qml/components/ShapeSelector.qml")
    list(APPEND QML_FILES src/qml/components/ShapeSelector.qml)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/qml/components/InputDeviceSelector.qml")
    list(APPEND QML_FILES src/qml/components/InputDeviceSelector.qml)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/qml/components/SessionControls.qml")
    list(APPEND QML_FILES src/qml/components/SessionControls.qml)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/qml/components/TransformDisplay.qml")
    list(APPEND QML_FILES src/qml/components/TransformDisplay.qml)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/qml/components/Viewport3D.qml")
    list(APPEND QML_FILES src/qml/components/Viewport3D.qml)
endif()

# Print which QML files will be included
message(STATUS "QML files to be included:")
foreach(qml_file ${QML_FILES})
    message(STATUS "  ${qml_file}")
endforeach()

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

# Link libraries - NOW INCLUDING HIDAPI
target_link_libraries(ManualRegistrationGL_V2 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::OpenGL
    ${OpenCV_LIBS}
    ${HIDAPI_LIBRARIES}
)

# Include directories
target_include_directories(ManualRegistrationGL_V2 PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${HIDAPI_INCLUDE_DIRS}
)

# Compiler definitions
target_compile_definitions(ManualRegistrationGL_V2 PRIVATE
    SPACEMOUSE_SUPPORT_ENABLED
)

# Set output directory
set_target_properties(ManualRegistrationGL_V2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Print configuration
message(STATUS "Manual Registration Simulator V2 Configuration:")
message(STATUS "  OpenCV version: ${OpenCV_VERSION}")
message(STATUS "  Qt6 version: ${Qt6_VERSION}")
message(STATUS "  HIDAPI found: ${HIDAPI_FOUND}")
message(STATUS "  SpaceMouse support: ENABLED")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output directory: ${CMAKE_BINARY_DIR}")