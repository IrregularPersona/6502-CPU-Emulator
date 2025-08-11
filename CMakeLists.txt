cmake_minimum_required(VERSION 3.16)

project(6502-CPU-Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output binaries to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(main
  src/main.cpp
)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()

