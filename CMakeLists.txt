cmake_minimum_required(VERSION 3.16)
project(insight)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 11.5)
endif()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Source)
