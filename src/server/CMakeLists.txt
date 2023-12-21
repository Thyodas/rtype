cmake_minimum_required(VERSION 3.17)

project(server)

set(SRCS
    Main.cpp
    ../common/network/Server.cpp
)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIRS})

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
else()
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -O3 -g")
endif()

include_directories(include)
include_directories(../../include)

add_executable(server ${SRCS} ${HEADERS})

target_link_libraries(server ${Boost_LIBRARIES} Boost::serialization)
