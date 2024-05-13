@echo off
setlocal enabledelayedexpansion

:: Set the project root directory
set PROJECT_ROOT=%~dp0..\

:: Set the build directory
set BUILD_DIR=%PROJECT_ROOT%\build

:: Set the PATH environment variable to include the Python library directory
set PATH=%PATH%;C:\Python310\Lib\site-packages

:: Run the RAS test executable
echo Running RAS tests...
"%BUILD_DIR%\ras_test.exe"

:: Check the test results
if %ERRORLEVEL% == 0 (
    echo All tests passed.
) else (
    echo Some tests failed.
    exit /b 1
)