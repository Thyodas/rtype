\page install_readme Installation
# Install

This document provides an overview of the project and instructions on how to build and use it.

## Table of Contents

- [Install](#install)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
  - [Building the Project](#building-the-project)
    - [Dependencies](#dependencies)
    - [Building on Windows](#building-on-windows)
      - [Installing CMake on Windows](#installing-cmake-on-windows)
    - [Building on Unix-like Systems](#building-on-unix-like-systems)
  - [Running the Project](#running-the-project)
    - [Server](#server)
    - [Client](#client)
    - [Game Engine](#game-engine)

## Project Overview

The project consists of a server and a client component. The server handles some functionality, and the client interacts with it.

## Building the Project

### Dependencies

The project relies on the following dependencies:

- [Raylib 5.0](https://www.raylib.com/)
- [Boost](https://www.boost.org/)
- [CMake](https://cmake.org/) (for building the project)

### Building on Windows

#### Installing CMake on Windows

Before building the project, you need to install CMake. Follow the [CMake Installation Guide for Windows](#cmake-installation-guide-for-windows) to install CMake on your Windows machine.

After installing CMake, you can proceed with building the project.

1. Install Visual Studio.
2. Open a command prompt and navigate to the project root directory.
3. Run the following commands:
   ```bash
   git submodule init
   git pull --recurse-submodules
   .\vcpkg\bootstrap-vcpkg.bat
   .\vcpkg\vcpkg integrate install
   cmake -B build
   cmake --build build
    ```

### Building on Unix-like Systems

1. Install CMake and a C++ compiler (e.g., g++).
2. Open a terminal and navigate to the project root directory.
3. Run the following commands:
    ```bash
    git submodule init
    git pull --recurse-submodules
    cmake -B build
    cmake --build build
    ```

## Running the Project
### Server

1. After building, navigate to the build directory.
2. Run the server executable:
    ```bash
    ./build/server
    ```

> [!NOTE]
> On Windows change '/' to '\' and add '.exe' to the end of the executable name.

### Client

1. After building, navigate to the build directory.
2. Run the client executable:
    ```bash
    cd src/client
    ../../build/client
    ```

> [!NOTE]
>  On Windows change '/' to '\' and add '.exe' to the end of the executable name.


### Game Engine
