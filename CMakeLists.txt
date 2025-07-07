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

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Main application source
set(SOURCES
    src/main.cpp
)

# Create the main executable
qt6_add_executable(ManualRegistrationGL_V2 ${SOURCES})

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

# Enable testing
enable_testing()
add_subdirectory(tests)
