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

# Build the UART driver
echo "Building UART driver..."
$CC $CFLAGS -c "$SRC_DIR/driver/uart.c" -o "$BUILD_DIR/uart.o"

# Build the RAS library
echo "Building RAS library..."
$CC $CFLAGS -I"$SRC_DIR/ras/include" -I"$SRC_DIR/driver" -c "$SRC_DIR/ras/src/ras.c" -o "$BUILD_DIR/ras.o"
ar rcs "$BUILD_DIR/libras.a" "$BUILD_DIR/uart.o" "$BUILD_DIR/ras.o"

# Build the RAS test executable
echo "Building RAS test executable..."
$CC $CFLAGS -I"$SRC_DIR/ras/include" -I"$SRC_DIR/driver" "$SRC_DIR/test/ras_test.c" "$BUILD_DIR/libras.a" -o "$BUILD_DIR/ras_test"

echo "Build completed successfully."