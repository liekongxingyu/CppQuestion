cmake_minimum_required(VERSION 3.10)
project(NetFramework CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/Buffer.cc
    src/Socket.cc
    src/Epoll.cc
    src/TcpConnection.cc
    src/TcpServer.cc
    main.cc
)

add_executable(net_server ${SOURCES})
