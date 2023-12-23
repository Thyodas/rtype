#!/usr/bin/env bash

# Build the project
cmake -B "./build/" || (echo "cmake failed" && exit 1)

cmake --build build || (echo "Build failed" && exit 1)
