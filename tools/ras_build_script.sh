#!/bin/bash

# Set the project root directory
PROJECT_ROOT=$(dirname "$(dirname "$(readlink -f "$0")")")

# Set the source and build directories
SRC_DIR="$PROJECT_ROOT/code"
BUILD_DIR="$PROJECT_ROOT/build"

# Create the build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Set the compiler and flags
CC=gcc
CFLAGS="-Wall -Wextra -Werror -pedantic -std=c99 -O3"

# Get Python include and library flags
PYTHON_INCLUDE=$(python3.10-config --includes)
PYTHON_LIBS=$(python3.10-config --ldflags)
PYTHON_LIB_DIR=/opt/homebrew/lib/python3.10/site-packages
PYTHON_LIB=-lpython3.10

# Build the UART driver
echo "Building UART driver..."
$CC $CFLAGS -c "$SRC_DIR/driver/uart.c" -o "$BUILD_DIR/uart.o"

# Build the RAS library
echo "Building RAS library..."
$CC $CFLAGS $PYTHON_INCLUDE -I"$SRC_DIR/ras/include" -c "$SRC_DIR/ras/src/ras_python_interface.c" -o "$BUILD_DIR/ras_python_interface.o"
$CC $CFLAGS $PYTHON_INCLUDE -I"$SRC_DIR/ras/include" -c "$SRC_DIR/ras/src/ras.c" -o "$BUILD_DIR/ras.o"
$CC $CFLAGS $PYTHON_INCLUDE -I"$SRC_DIR/ras/include" -c "$SRC_DIR/driver/uart.c" -o "$BUILD_DIR/uart.o"
ar rcs "$BUILD_DIR/libras.a" "$BUILD_DIR/ras_python_interface.o" "$BUILD_DIR/ras.o" "$BUILD_DIR/uart.o"

# Build the RAS test executable
echo "Building RAS test executable..."
$CC $CFLAGS $PYTHON_INCLUDE -I"$SRC_DIR/ras/include" "$SRC_DIR/test/ras_test.c" "$BUILD_DIR/libras.a" $PYTHON_LIBS -L$PYTHON_LIB_DIR $PYTHON_LIB -o "$BUILD_DIR/ras_test"

echo "Build completed successfully."