# Manual Registration Simulator V2 - Requirements

This document lists all dependencies and system requirements for building and running the Manual Registration Simulator V2.

## System Requirements

### Operating System
- **Ubuntu 20.04 LTS or later** (recommended)
- **Ubuntu 22.04 LTS** (tested)
- **Ubuntu 24.04 LTS** (tested)
- Other Debian-based distributions may work

### Hardware Requirements
- **CPU**: Intel/AMD x64 processor
- **Memory**: 8GB RAM minimum, 16GB recommended
- **Graphics**: OpenGL 4.0+ compatible GPU
- **Storage**: 2GB free space
- **Optional**: 3Dconnexion SpaceMouse device

### Graphics Cards
- **NVIDIA**: GTX 1000 series or newer (tested with RTX 4070)
- **AMD**: RX 500 series or newer
- **Intel**: UHD Graphics or newer integrated graphics

## Dependencies

### Core Build Tools
```bash
build-essential
cmake (3.16+)
ninja-build
pkg-config
git
C++ Compilers
```
### C++ Compilers
```bash
gcc-11 / g++-11
clang-14
clangd-14
```
### Qt6 Framework
```bash
qt6-base-dev
qt6-base-dev-tools
libqt6opengl6-dev
qt6-declarative-dev
libqt6qml6-dev
libqt6quick6-dev
```
### Qt6 QML Modules
```bash
qml6-module-qtquick
qml6-module-qtquick-controls
qml6-module-qtquick-layouts
qml6-module-qtquick-window
qml6-module-qtquick-dialogs
```
### Graphics Libraries
```bash
mesa-common-dev
libgl1-mesa-dev
libglu1-mesa-dev
mesa-utils
freeglut3-dev
libegl1-mesa-dev
libgles2-mesa-dev
libglfw3-dev
libglew-dev
```
### Computer Vision
```bash
libopencv-dev (4.0+)
opencv-data
```
### Input Device Support
```bash
libhidapi-dev
libhidapi-hidraw0
libhidapi-libusb0
```
### Additional Libraries
```bash
libglm-dev          # OpenGL Mathematics
libassimp-dev       # 3D model loading
libspdlog-dev       # High-performance logging
nlohmann-json3-dev  # JSON handling
```
### Documentation Tools (Optional)
```bash
doxygen
graphviz
pandoc
```
## Graphics Drivers
### NVIDIA Graphics
```bash
nvidia-driver-535 (or latest)
nvidia-utils-535
```
### AMD Graphics
```bash
mesa-vulkan-drivers
mesa-va-drivers
mesa-vdpau-drivers
```
# Installation
### Automated Installation
```bash
./scripts/install_dependencies.sh
```
## Verification
After installation, verify your setup:
```bash
# Check Qt6
qmake6 --version

# Check OpenCV
pkg-config --modversion opencv4

# Check OpenGL
glxinfo | grep "OpenGL version"

# Check compiler
gcc --version
```
# Troubleshooting
## Common Issues
### Qt6 Not Found
```bash
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6:$CMAKE_PREFIX_PATH
```
### OpenGL Context Errors
```bash
# For hybrid graphics systems
sudo prime-select nvidia  # or intel
```
### SpaceMouse Permission Denied
```bash
# Check if user is in input group
groups $USER

# Add user to input group
sudo usermod -a -G input $USER
# Then logout/login
```
### Graphics Driver Issues
```bash
# Check graphics hardware
lspci | grep VGA

# Test OpenGL
glxinfo | grep "OpenGL"

# For NVIDIA systems
nvidia-smi
```
# Build Configuration
## CMake Options
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
### Environment Variables
```bash
export QT_SELECT=6
export MESA_GL_VERSION_OVERRIDE=3.3
export MESA_GLSL_VERSION_OVERRIDE=330
```
# Tested Configurations
### Ubuntu 24.04 LTS

- Qt6 6.4.2
- OpenCV 4.6.0
- GCC 11.4.0
- NVIDIA RTX 4070 with driver 550.163.01

### Ubuntu 22.04 LTS

- Qt6 6.2.4
- OpenCV 4.5.4
- GCC 11.2.0

### Support
For installation issues, please check:

- This requirements document
- The installation script output
- GitHub Issues for known problems
- Create a new issue with your system details

Run the installation script with:
```bash
./scripts/install_dependencies.sh
```