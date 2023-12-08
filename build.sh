#!/usr/bin/env bash

# Build the project
cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -S . -B "./build/" || exit

cd "./build/" || exit

make -j 20
make package -j 20
make package_source -j 20
