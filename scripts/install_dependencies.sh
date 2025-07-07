#!/bin/bash

# Manual Registration Simulator V2 - Dependency Installation Script
# This script installs all required dependencies for Ubuntu 20.04+

set -e  # Exit on any error

echo "=========================================="
echo "Manual Registration Simulator V2"
echo "Dependency Installation Script"
echo "=========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if running on Ubuntu
if ! command -v apt &> /dev/null; then
    print_error "This script is designed for Ubuntu/Debian systems with apt package manager."
    exit 1
fi

# Check Ubuntu version
UBUNTU_VERSION=$(lsb_release -rs 2>/dev/null || echo "unknown")
print_status "Detected Ubuntu version: $UBUNTU_VERSION"

if [[ "$UBUNTU_VERSION" < "20.04" ]]; then
    print_warning "Ubuntu 20.04+ recommended. Your version: $UBUNTU_VERSION"
fi

# Update package lists
print_status "Updating package lists..."
sudo apt update

print_status "Upgrading existing packages..."
sudo apt upgrade -y

# Install essential build tools
print_status "Installing essential build tools..."
sudo apt install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    git \
    curl \
    wget \
    unzip

print_success "Build tools installed"

# Install modern C++ compilers
print_status "Installing C++ compilers..."
sudo apt install -y \
    gcc-11 \
    g++-11 \
    clang-14 \
    clang-tools-14 \
    clangd-14

# Set default compiler versions
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60 || true
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 60 || true
sudo update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-14 60 || true

print_success "C++ compilers installed"

# Install Qt6 Base Components
print_status "Installing Qt6 base components..."
sudo apt install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    libqt6opengl6-dev

print_success "Qt6 base components installed"

# Install Qt6 QML Components
print_status "Installing Qt6 QML components..."
sudo apt install -y \
    qt6-declarative-dev \
    libqt6qml6-dev \
    libqt6quick6-dev \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    qml6-module-qtquick-window \
    qml6-module-qtquick-dialogs

print_success "Qt6 QML components installed"

# Install Graphics and OpenGL Libraries
print_status "Installing graphics and OpenGL libraries..."
sudo apt install -y \
    mesa-common-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    mesa-utils \
    freeglut3-dev \
    libegl1-mesa-dev \
    libgles2-mesa-dev \
    libglfw3-dev \
    libglew-dev

print_success "Graphics libraries installed"

# Install OpenCV
print_status "Installing OpenCV..."
sudo apt install -y \
    libopencv-dev \
    opencv-data

print_success "OpenCV installed"

# Install SpaceMouse Support (HID API)
print_status "Installing SpaceMouse support..."
sudo apt install -y \
    libhidapi-dev \
    libhidapi-hidraw0 \
    libhidapi-libusb0

print_success "SpaceMouse support installed"

# Install Additional Math and Utility Libraries
print_status "Installing additional libraries..."
sudo apt install -y \
    libglm-dev \
    libassimp-dev \
    libspdlog-dev \
    nlohmann-json3-dev

print_success "Additional libraries installed"

# Install Documentation Tools
print_status "Installing documentation tools..."
sudo apt install -y \
    doxygen \
    graphviz \
    pandoc

print_success "Documentation tools installed"

# Configure SpaceMouse USB permissions
print_status "Configuring SpaceMouse USB permissions..."
sudo tee /etc/udev/rules.d/50-spacemouse.rules > /dev/null << 'UDEV_EOF'
# 3Dconnexion SpaceMouse devices
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c62e", MODE="0666", GROUP="input"
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c626", MODE="0666", GROUP="input"
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c627", MODE="0666", GROUP="input"
UDEV_EOF

# Reload udev rules
sudo udevadm control --reload-rules
sudo udevadm trigger

# Add user to input group
sudo usermod -a -G input $USER

print_success "SpaceMouse permissions configured"

# Check NVIDIA graphics (if present)
if lspci | grep -i nvidia &> /dev/null; then
    print_status "NVIDIA graphics detected. Checking drivers..."
    
    if command -v nvidia-smi &> /dev/null; then
        NVIDIA_VERSION=$(nvidia-smi --query-gpu=driver_version --format=csv,noheader,nounits | head -1)
        print_success "NVIDIA driver installed: $NVIDIA_VERSION"
    else
        print_warning "NVIDIA graphics detected but drivers not installed."
        print_status "Installing NVIDIA drivers..."
        sudo apt install -y nvidia-driver-535 nvidia-utils-535
        print_warning "NVIDIA drivers installed. Please reboot before using the simulator."
    fi
fi

# Set up environment variables
print_status "Setting up environment variables..."
BASHRC_ADDITIONS="
# Manual Registration Simulator V2 environment
export QT_SELECT=6
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6:\$CMAKE_PREFIX_PATH
export MESA_GL_VERSION_OVERRIDE=3.3
export MESA_GLSL_VERSION_OVERRIDE=330
"

if ! grep -q "Manual Registration Simulator V2" ~/.bashrc; then
    echo "$BASHRC_ADDITIONS" >> ~/.bashrc
    print_success "Environment variables added to ~/.bashrc"
else
    print_status "Environment variables already configured"
fi

# Verify installations
print_status "Verifying installations..."

# Check Qt6
if qmake6 --version &> /dev/null; then
    QT_VERSION=$(qmake6 --version | grep "Qt version" | cut -d' ' -f4)
    print_success "Qt6 verified: $QT_VERSION"
else
    print_error "Qt6 verification failed"
fi

# Check OpenCV
if pkg-config --exists opencv4; then
    OPENCV_VERSION=$(pkg-config --modversion opencv4)
    print_success "OpenCV verified: $OPENCV_VERSION"
else
    print_error "OpenCV verification failed"
fi

# Check OpenGL
if command -v glxinfo &> /dev/null; then
    if glxinfo | grep "OpenGL version" &> /dev/null; then
        GL_VERSION=$(glxinfo | grep "OpenGL version" | cut -d' ' -f4)
        print_success "OpenGL verified: $GL_VERSION"
    else
        print_warning "OpenGL verification failed - may need graphics driver configuration"
    fi
else
    print_warning "glxinfo not available for OpenGL verification"
fi

# Final instructions
echo ""
echo "=========================================="
print_success "Dependency installation completed!"
echo "=========================================="
echo ""
print_status "Next steps:"
echo "1. Restart your terminal (or run: source ~/.bashrc)"
echo "2. If you installed NVIDIA drivers, reboot your system"
echo "3. Build the project:"
echo "   mkdir build && cd build"
echo "   cmake .."
echo "   make -j\$(nproc)"
echo ""
print_status "For SpaceMouse support, you may need to logout/login for group changes to take effect."
echo ""
print_success "Installation complete! You're ready to build Manual Registration Simulator V2."
