@echo off
setlocal enabledelayedexpansion

:: Set the project root directory
set PROJECT_ROOT=%~dp0..\

:: Set the source and build directories
set SRC_DIR=%PROJECT_ROOT%\code
set BUILD_DIR=%PROJECT_ROOT%\build

:: Create the build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Set the compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -O3

:: Get Python include and library flags
set PYTHON_INCLUDE=C:\Python310\include
set PYTHON_LIBS=-L"C:\Python310\libs" -lpython310
set PYTHON_LIB_DIR=C:\Python310\Lib\site-packages

:: Build the UART driver
echo Building UART driver...
%CC% %CFLAGS% -c "%SRC_DIR%\driver\uart.c" -o "%BUILD_DIR%\uart.o"

:: Build the RAS library
echo Building RAS library...
%CC% %CFLAGS% %PYTHON_INCLUDE% -I"%SRC_DIR%\ras\include" -c "%SRC_DIR%\ras\src\ras_python_interface.c" -o "%BUILD_DIR%\ras_python_interface.o"
%CC% %CFLAGS% %PYTHON_INCLUDE% -I"%SRC_DIR%\ras\include" -c "%SRC_DIR%\ras\src\ras.c" -o "%BUILD_DIR%\ras.o"
%CC% %CFLAGS% %PYTHON_INCLUDE% -I"%SRC_DIR%\ras\include" -c "%SRC_DIR%\driver\uart.c" -o "%BUILD_DIR%\uart.o"
ar rcs "%BUILD_DIR%\libras.a" "%BUILD_DIR%\ras_python_interface.o" "%BUILD_DIR%\ras.o" "%BUILD_DIR%\uart.o"

:: Build the RAS test executable
echo Building RAS test executable...
%CC% %CFLAGS% %PYTHON_INCLUDE% -I"%SRC_DIR%\ras\include" "%SRC_DIR%\test\ras_test.c" "%BUILD_DIR%\libras.a" %PYTHON_LIBS% -L%PYTHON_LIB_DIR% %PYTHON_LIB% -o "%BUILD_DIR%\ras_test"

echo Build completed successfully.