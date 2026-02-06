@echo off
REM Build script for NS Basic/Palm OS Windows Executable
REM Requires: Python 3.8+, PyInstaller

echo.
echo ================================================================
echo NS Basic/Palm OS - Windows Build Script
echo ================================================================
echo.

REM Check if PyInstaller is installed
python -c "import PyInstaller" 2>NUL
if errorlevel 1 (
    echo Installing PyInstaller...
    pip install pyinstaller
)

REM Install dependencies
echo Installing dependencies...
pip install -r ../requirements.txt

REM Clean previous builds
echo Cleaning previous builds...
if exist build rmdir /s /q build
if exist dist rmdir /s /q dist

REM Build executable
echo.
echo Building NS Basic/Palm executable...
echo.
pyinstaller --clean nsbasic_palm.spec

REM Check if build succeeded
if exist dist\NSBasicPalm\NSBasicPalm.exe (
    echo.
    echo ================================================================
    echo BUILD SUCCESSFUL!
    echo ================================================================
    echo.
    echo Executable location: dist\NSBasicPalm\NSBasicPalm.exe
    echo.
    echo To test the executable:
    echo    cd dist\NSBasicPalm
    echo    NSBasicPalm.exe
    echo.
) else (
    echo.
    echo ================================================================
    echo BUILD FAILED!
    echo ================================================================
    echo.
    echo Check the output above for errors.
    echo.
)

pause
