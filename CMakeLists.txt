cmake_minimum_required(VERSION 3.16)
project(ChatServer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SERVER_SOURCES src/*.cpp)
add_executable(chat_server ${SERVER_SOURCES})
target_include_directories(chat_server PRIVATE src)
target_link_libraries(chat_server PRIVATE Threads::Threads)