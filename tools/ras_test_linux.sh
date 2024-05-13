#!/bin/bash

# Set the project root directory
PROJECT_ROOT=$(dirname "$(dirname "$(readlink -f "$0")")")

# Set the build directory
BUILD_DIR="$PROJECT_ROOT/build"

# Set the LD_LIBRARY_PATH environment variable to include the Python library directory
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PYTHON_LIB_DIR

# Run the RAS test executable
echo "Running RAS tests..."
"$BUILD_DIR/ras_test"

# Check the test results
if [ $? -eq 0 ]; then
    echo "All tests passed."
else
    echo "Some tests failed."
    exit 1
fi