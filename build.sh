#!/bin/bash

# Manual Registration Simulator V2 - Build Script
# This script builds the project with proper error handling

set -e  # Exit on any error

echo "=========================================="
echo "Manual Registration Simulator V2"
echo "Build Script"
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

# Check CMake
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Please install CMake."
    print_status "Run: sudo apt install cmake"
    exit 1
fi

print_success "All dependencies found"

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
print_status "Configuring with CMake..."
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

# Return to project root
cd ..

# Print usage instructions
echo ""
echo "=========================================="
print_success "Build Complete!"
echo "=========================================="
echo ""
print_status "To run the application:"
echo "  cd build"
echo "  ./ManualRegistrationGL_V2"
echo ""
print_status "Available shapes:"
echo "  1 - Cube"
echo "  2 - Sphere" 
echo "  3 - Torus"
echo "  4 - Tetrahedron (default)"
echo ""
print_status "Mouse controls:"
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
echo "  - Task timing and completion detection"
echo ""
echo "🚀 Ready to use for 3D registration research!"
