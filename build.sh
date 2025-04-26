#!/bin/bash

# Update submodules
git submodule update --init

# Apply bitsery patch
git apply lib/bitsery_fix.patch

# Build
cmake -B build

# Compile
cmake --build build
