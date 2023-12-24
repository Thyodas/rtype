# Install

This document provides an overview of the project and instructions on how to build and use it.

## Table of Contents

- [Project Overview](#project-overview)
- [Building the Project](#building-the-project)
  - [Dependencies](#dependencies)
  - [Building on Windows](#building-on-windows)
  - [Building on Unix-like Systems](#building-on-unix-like-systems)
- [Running the Project](#running-the-project)
  - [Server](#server)
  - [Client](#client)

## Project Overview

The project consists of a server and a client component. The server handles some functionality, and the client interacts with it.

## Building the Project

### Dependencies

The project relies on the following dependencies:

- [Raylib 5.0](https://www.raylib.com/)
- [Boost](https://www.boost.org/)

### Building on Windows

1. Install Visual Studio.
2. Install vcpkg by following the instructions [here](https://github.com/microsoft/vcpkg).
3. Open a command prompt and navigate to the project root directory.
4. Run the following commands:
   ```bash
   cmake -B build -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>\scripts\buildsystems\vcpkg.cmake
   cmake --build build
    ```

### Building on Unix-like Systems

1. Install CMake and a C++ compiler (e.g., g++).
2. Open a terminal and navigate to the project root directory.
3. Run the following commands:
    ```bash
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake
    cmake --build build
    ```

## Running the Project
### Server

1. After building, navigate to the build directory.
2. Run the server executable:
    ```bash
    ./server
    ```

### Client

1. After building, navigate to the build directory.
2. Run the client executable:
    ```bash
    ./client
    ```
