@echo off
:: Automatic coverage generation and report opening on Windows

:: Title
echo ================================================
echo        Automatic Code Coverage - FTPGestion
echo ================================================

:: Colors for feedback (simulated with echo)
echo Cleaning old build and coverage files...

:: Clean previous build
rmdir /s /q build
del /q coverage.info coverage_filtered.info
rmdir /s /q coverage_report

:: Create build directory
echo Creating build folder...
mkdir build
cd build

:: Configure with CMake using MinGW
echo Configuring project with CMake...
cmake .. -G "MinGW Makefiles"
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

:: Build project with MinGW
echo Building project...
mingw32-make
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

:: Run the executable
echo Running the program...
FTPGestion.exe

:: Generate coverage info
echo Capturing coverage data...
lcov --capture --directory . --output-file coverage.info

:: Remove unwanted system files
echo Cleaning system and external files from coverage...
lcov --remove coverage.info "C:/Program Files/*" "C:/MinGW/*" --output-file coverage_filtered.info

:: Generate HTML report
echo Generating HTML report...
genhtml coverage_filtered.info --output-directory coverage_report

:: Automatically open the coverage report in the browser
echo Opening coverage report...
start "" coverage_report\index.html

:: Done
echo ================================================
echo Coverage report generated and opened successfully!
echo ================================================
pause
exit /b
