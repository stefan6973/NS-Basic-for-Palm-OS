#!/bin/bash
# Build script for NS Basic/Palm OS (Linux/Mac testing)
# For final deployment, use build_windows.bat on Windows

echo ""
echo "================================================================"
echo "NS Basic/Palm OS - Build Script"
echo "================================================================"
echo ""

# Check if PyInstaller is installed
python3 -c "import PyInstaller" 2>/dev/null
if [ $? -ne 0 ]; then
    echo "Installing PyInstaller..."
    pip3 install pyinstaller
fi

# Install dependencies
echo "Installing dependencies..."
pip3 install -r ../requirements.txt

# Clean previous builds
echo "Cleaning previous builds..."
rm -rf build dist

# Build executable
echo ""
echo "Building NS Basic/Palm executable..."
echo ""
pyinstaller --clean nsbasic_palm.spec

# Check if build succeeded
if [ -f "dist/NSBasicPalm/NSBasicPalm" ]; then
    echo ""
    echo "================================================================"
    echo "BUILD SUCCESSFUL!"
    echo "================================================================"
    echo ""
    echo "Executable location: dist/NSBasicPalm/NSBasicPalm"
    echo ""
    echo "To test the executable:"
    echo "    cd dist/NSBasicPalm"
    echo "    ./NSBasicPalm"
    echo ""
else
    echo ""
    echo "================================================================"
    echo "BUILD FAILED!"
    echo "================================================================"
    echo ""
    echo "Check the output above for errors."
    echo ""
fi
