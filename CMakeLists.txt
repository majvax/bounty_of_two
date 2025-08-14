cmake_minimum_required(VERSION 3.30)


if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)


project(
    bounty_of_two
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL ""
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(Dependencies.cmake)

# Add source tree
add_subdirectory(src)
