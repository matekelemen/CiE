#!/bin/bash

set -e

# Get source and destination directories
SOURCE_DIR="$(dirname "$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )")"
BUILD_DIR="${SOURCE_DIR}/build/Debug"
INSTALL_DIR="${BUILD_DIR}/install"

# Make sure PWD is in source
cd $SOURCE_DIR

# Check whether a clean build was requested
cleanBuild=0

if [ $# -ne 0 ]; then
    if [ $1 == "clean" ]; then
        cleanBuild=1
    else
        echo -e "\e[0;31mInvalid argument: $1"
        exit 1
    fi
fi

# Create destination directories
if [ $cleanBuild -eq 1 ]; then
    rm -rf "${INSTALL_DIR}" "${BUILD_DIR}"
fi

mkdir -p "${BUILD_DIR}" "${INSTALL_DIR}"

# Set compiler
export CC=/bin/gcc-10
export CXX=/bin/g++-10

# Configure CMake
unbuffer cmake  -H${SOURCE_DIR}                                     \
                -B${BUILD_DIR}                                      \
                                                                    \
                -DCMAKE_BUILD_TYPE="Debug"                          \
                -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}"             \
                                                                    \
                -DCIE_INSTALL_OUTPUT_PREFIX="$INSTALL_DIR/output"   \
                -DCIE_ENABLE_DEBUG_FILE_OUTPUT=ON                   \
                -DCIE_ENABLE_DIVISION_BY_ZERO_CHECKS=ON             \
                -DCIE_ENABLE_EXCEPTION_TRACING=ON                   \
                -DCIE_ENABLE_OUT_OF_RANGE_TESTS=ON                  \
                -DCIE_ENABLE_RUNTIME_GEOMETRY_CHECKS=ON             \
                -DCIE_ENABLE_OPENMP=ON                              \
                -DCIE_BUILD_TESTS=ON                                \
                                                                    \
                | tee "${BUILD_DIR}/cmake_output.log"

# Build
numberOfCores=$(grep -c ^processor /proc/cpuinfo)
numberOfCores=$((numberOfCores-1))
cd $BUILD_DIR
unbuffer make install -j$numberOfCores | tee "${BUILD_DIR}/compiler.log"