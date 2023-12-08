#!/usr/bin/env bash

# Build the project
cmake -S . -B "./build/" || exit

cd "./build/" || exit

make -j 20|| exit
make package -j 20
make package_source -j 20
