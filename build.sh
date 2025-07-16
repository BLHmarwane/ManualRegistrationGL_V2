#!/bin/bash

# Manual Registration Simulator V2 - Build Script with SpaceMouse Support
# This script builds the project with proper error handling and SpaceMouse integration

set -e  # Exit on any error

echo "=========================================="
echo "Manual Registration Simulator V2"
echo "Build Script with SpaceMouse Support"
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

# Check if we're in the project root
if [[ ! -f "CMakeLists.txt" ]]; then
    print_error "Please run this script from the project root directory (where CMakeLists.txt is located)"
    exit 1
fi

# Check for required dependencies
print_status "Checking dependencies..."

# Check Qt6
if ! command -v qmake6 &> /dev/null; then
    print_error "Qt6 not found. Please install Qt6 development packages."
    print_status "Run: sudo apt install qt6-base-dev qt6-declarative-dev"
    exit 1
fi

# Check OpenCV
if ! pkg-config --exists opencv4; then
    print_error "OpenCV not found. Please install OpenCV development packages."
    print_status "Run: sudo apt install libopencv-dev"
    exit 1
fi

# Check HIDAPI for SpaceMouse support
if ! pkg-config --exists hidapi-hidraw; then
    print_error "HIDAPI not found. Installing HIDAPI for SpaceMouse support..."
    sudo apt update
    sudo apt install -y libhidapi-dev libhidapi-hidraw0 libhidapi-libusb0

    if ! pkg-config --exists hidapi-hidraw; then
        print_error "Failed to install HIDAPI. SpaceMouse support will be disabled."
        exit 1
    fi
    print_success "HIDAPI installed successfully"
fi

# Check CMake
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Please install CMake."
    print_status "Run: sudo apt install cmake"
    exit 1
fi

print_success "All dependencies found"

# Setup SpaceMouse permissions
print_status "Setting up SpaceMouse permissions..."

# Copy udev rules if they exist
if [[ -f "scripts/99-spacemouse.rules" ]]; then
    sudo cp scripts/99-spacemouse.rules /etc/udev/rules.d/
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    print_success "SpaceMouse udev rules installed"
else
    print_warning "SpaceMouse udev rules not found - creating them..."

    # Create basic udev rules
    sudo tee /etc/udev/rules.d/99-spacemouse.rules > /dev/null << 'EOF'
# 3DConnexion SpaceMouse devices
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c626", MODE="0666", GROUP="input"
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c627", MODE="0666", GROUP="input"
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c62e", MODE="0666", GROUP="input"
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c62f", MODE="0666", GROUP="input"
EOF

    sudo udevadm control --reload-rules
    sudo udevadm trigger
    print_success "Basic SpaceMouse udev rules created"
fi

# Add user to input group
if ! groups $USER | grep -q '\binput\b'; then
    sudo usermod -a -G input $USER
    print_warning "Added $USER to input group - please logout/login for changes to take effect"
else
    print_success "User already in input group"
fi

# Clean previous build
print_status "Cleaning previous build..."
if [[ -d "build" ]]; then
    rm -rf build/
    print_success "Cleaned build directory"
fi

# Create build directory
print_status "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
print_status "Configuring with CMake (with SpaceMouse support)..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [[ $? -ne 0 ]]; then
    print_error "CMake configuration failed!"
    exit 1
fi

print_success "CMake configuration completed"

# Build the project
print_status "Building project..."
make -j$(nproc)

if [[ $? -ne 0 ]]; then
    print_error "Build failed!"
    exit 1
fi

print_success "Build completed successfully!"

# Check if executable was created
if [[ -f "ManualRegistrationGL_V2" ]]; then
    print_success "Executable created: build/ManualRegistrationGL_V2"
else
    print_error "Executable not found after build!"
    exit 1
fi

# Test SpaceMouse functionality (if test was built)
if [[ -f "test_spacemouse" ]]; then
    print_status "Testing SpaceMouse functionality..."
    ./test_spacemouse
    print_success "SpaceMouse tests completed"
fi

# Return to project root
cd ..

# Check for SpaceMouse devices
print_status "Scanning for SpaceMouse devices..."
SPACEMOUSE_FOUND=false

if command -v lsusb &> /dev/null; then
    if lsusb | grep -E "(046d|256f)" | grep -v "Unifying\|Receiver" > /dev/null; then
        SPACEMOUSE_FOUND=true
        print_success "SpaceMouse device detected:"
        lsusb | grep -E "(046d|256f)" | grep -v "Unifying\|Receiver"
    fi
fi

if [[ "$SPACEMOUSE_FOUND" == "false" ]]; then
    print_warning "No SpaceMouse device detected"
    print_status "Application will work with mouse input mode only"
fi

# Print usage instructions
echo ""
echo "=========================================="
print_success "Build Complete with SpaceMouse Support!"
echo "=========================================="
echo ""
print_status "To run the application:"
echo "  cd build"
echo "  ./ManualRegistrationGL_V2"
echo ""
print_status "Interaction Modes:"
echo "  1. Traditional Mouse - Always available"
echo "  2. SpaceMouse (6DOF) - Available when device connected"
echo "  3. Multi-Touch      - Coming in future update"
echo ""
print_status "SpaceMouse Controls:"
echo "  Translation - X/Y/Z movement"
echo "  Rotation    - Roll/Pitch/Yaw"
echo "  Left Button - Reset transform"
echo "  Right Button- Cycle interaction modes"
echo ""
print_status "Mouse Controls:"
echo "  Left drag   - Rotate object"
echo "  Right drag  - Translate object (XY)"
echo "  Middle drag - Scale object"
echo "  Wheel       - Scale object"
echo "  Ctrl+Wheel  - Translate Z"
echo ""
print_status "Keyboard shortcuts:"
echo "  WASD/Arrows - Translate object"
echo "  Q/E         - Translate Z"
echo "  +/-         - Scale"
echo "  1/2/3/4     - Select shape"
echo "  Ctrl+R      - Reset transform"
echo ""
print_status "Research features:"
echo "  - Dual model display (reference + movable)"
echo "  - Vertex markers for alignment"
echo "  - Real-time accuracy calculation"
echo "  - Multi-modal interaction comparison"
echo "  - SpaceMouse input visualization"
echo "  - Research-grade data logging"
echo ""

if [[ "$SPACEMOUSE_FOUND" == "true" ]]; then
    echo "🚀 Ready for 6DOF SpaceMouse research!"
else
    echo "🖱️  Ready for traditional mouse research!"
    echo ""
    print_status "To add SpaceMouse support:"
    echo "  1. Connect your SpaceMouse device"
    echo "  2. Restart the application"