cmake_minimum_required(VERSION 3.10)
project(CineBrew)

# vcpkg manifest mode
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory for all executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(src)

# SDL2 optional dependency removed for now (use stub GUI implementation)
# If you later want SDL2, re-enable find_package(SDL2 CONFIG REQUIRED)

# Runtime Library
add_library(runtime STATIC
    src/runtime/runtime.cpp
)

# VM Library
add_library(vm STATIC
    src/vm/vm.cpp
)
target_link_libraries(vm runtime)

# Compiler Library (Lexer + Parser + Semantic + CodeGen)
add_library(compiler STATIC
    src/compiler/lexer.cpp
    src/compiler/token.cpp
    src/compiler/parser.cpp
    src/compiler/ast.cpp
    src/compiler/semantic.cpp
    src/compiler/codegen.cpp
    src/compiler/compiler.cpp
)

# VM Test programs
# vm_test removed; use unit tests in `tests/` where applicable

add_executable(test_vm_basic tests/test_vm_basic.cpp)
target_link_libraries(test_vm_basic vm runtime gui)

add_executable(test_vm_factorial tests/test_vm_factorial.cpp)
target_link_libraries(test_vm_factorial vm runtime gui)

add_executable(test_lexer tests/test_lexer.cpp)
target_link_libraries(test_lexer compiler)

add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser compiler)

# Semantic Analyzer programs
add_executable(test_semantic tests/test_semantic.cpp)
target_link_libraries(test_semantic compiler runtime gui)

# Code Generator tests
add_executable(test_codegen tests/test_codegen.cpp)
target_link_libraries(test_codegen compiler vm runtime gui)

# Runtime tests
add_executable(test_runtime tests/test_runtime.cpp)
target_link_libraries(test_runtime compiler vm runtime gui)

# GUI Library
add_library(gui STATIC
    src/gui/game_loop.cpp
    src/gui/window.cpp
)
target_link_libraries(gui)

# Game Loop tests
add_executable(test_gameloop tests/test_gameloop.cpp)
target_link_libraries(test_gameloop compiler vm runtime gui)

# Game Runner target removed; use `cinebrew` entrypoint to run games

# Complete Compiler
add_executable(cinebrew src/compiler/cinebrew_main.cpp)
target_link_libraries(cinebrew compiler vm runtime gui)

