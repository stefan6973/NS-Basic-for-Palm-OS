#!/bin/bash
# Build script for NS Basic/Palm OS (Linux/Debian 13)
# For final deployment, use build_windows.bat on Windows

set -e  # Exit on error

echo ""
echo "================================================================"
echo " NS Basic/Palm OS - Build Script (Debian 13)"
echo "================================================================"
echo ""

echo ""
echo "Checking if running on Debian ..."
if [ -f /etc/os-release ]; then
    . /etc/os-release
    if [ "$ID" != "debian" ]; then
        echo "Warning: This script is optimized for Debian 13"
        echo "Current distribution: $PRETTY_NAME"
        echo ""
    fi
fi

# Check for required system packages
echo ""
echo "Checking system dependencies ..."
MISSING_PKGS=""
for pkg in python3 python3-pip python3-venv; do
    if ! dpkg -l | grep -q "^ii  $pkg "; then
        MISSING_PKGS="$MISSING_PKGS $pkg"
    fi
done

if [ -n "$MISSING_PKGS" ]; then
    echo "Missing system packages:$MISSING_PKGS"
    echo "Please install them with:"
    echo "    sudo apt-get update"
    echo "    sudo apt-get install$MISSING_PKGS"
    exit 1
fi

# Use a virtual environment (recommended on Debian 13)
if [ ! -d "venv" ]; then
    echo "Creating Python virtual environment ..."
    python3 -m venv venv
fi

echo ""
echo "Activating virtual environment ..."
source venv/bin/activate

# Upgrade pip (continue on error if already up-to-date)
#echo "Upgrading pip..."
#pip install --upgrade pip | grep -v 'already satisfied'

# Check if PyInstaller is installed
echo ""
echo "Checking for PyInstaller ..."
./venv/bin/python -c "import PyInstaller" 2>/dev/null
if [ $? -ne 0 ]; then
    echo "Installing PyInstaller..."
    ./venv/bin/python -m pip install pyinstaller
fi

# Install dependencies
echo ""
echo "Installing dependencies ..."
pip install -r ../requirements.txt

# Clean previous builds
echo ""
echo "Cleaning previous builds ..."
rm -rf build dist

# Build executable
echo ""
echo "Building NS Basic/Palm executable ..."
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
    echo "Note: Virtual environment is in python/venv/"
    echo "To deactivate: deactivate"
    echo ""
else
    echo ""
    echo "================================================================"
    echo "BUILD FAILED!"
    echo "================================================================"
    echo ""
    echo "Check the output above for errors."
    echo ""
    deactivate
    exit 1
fi
