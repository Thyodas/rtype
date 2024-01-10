#!/usr/bin/env bash

# Build the project
(cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release) || (echo "Build failed" && exit 1)

([[ -e ./build/Release ]] && cp ./build/Release/* ./build) || ([[ -e ./build/Debug ]] && cp ./build/Debug/* ./build)

cp -r ./ressources ./build/ || (echo "Copy ressources failed" && exit 1)

tar -C $(pwd) -cf package.tar build || (echo "Tar failed" && exit 1)
