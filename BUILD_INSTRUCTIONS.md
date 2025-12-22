# Building CINEBREW

## Quick Start

### Option 1: Using CMake (Recommended)

**Prerequisites:**
- CMake 3.10 or higher
- C++17 compiler (Visual Studio, MinGW, or Clang)

**Build Steps:**

1. **Install CMake** (if not installed):
   - Download from: https://cmake.org/download/
   - Or use package manager:
     - Windows: `choco install cmake` or `winget install cmake`
     - Or download installer from cmake.org

2. **Configure and Build:**
   ```powershell
   # Create build directory
   mkdir build
   cd build
   
   # Configure (choose one based on your compiler)
   cmake ..
   # OR for Visual Studio:
   cmake .. -G "Visual Studio 17 2022"
   # OR for MinGW:
   cmake .. -G "MinGW Makefiles"
   
   # Build
   cmake --build .
   # OR on Windows with Visual Studio:
   cmake --build . --config Release
   ```

3. **Run:**
   ```powershell
   # The executables will be in build/bin/
   .\build\bin\cinebrew.exe examples\hello.cb
   .\build\bin\game_runner.exe examples\pong_game.cb
   ```

### Option 2: Manual Compilation (Without CMake)

If CMake is not available, you can compile manually:

**Windows (PowerShell):**

```powershell
# Compile compiler
g++ -std=c++17 -Isrc src/compiler/*.cpp src/vm/vm.cpp src/runtime/runtime.cpp -o cinebrew.exe

# Compile game runner
g++ -std=c++17 -Isrc src/gui/game_runner.cpp src/compiler/*.cpp src/vm/vm.cpp src/runtime/runtime.cpp src/gui/*.cpp -o game_runner.exe
```

**Note:** This is a simplified command. You may need to adjust based on your compiler and include paths.

## Executable Locations

After building, executables will be in:
- `build/bin/cinebrew.exe` - Main compiler
- `build/bin/game_runner.exe` - Game runner
- `build/bin/test_*.exe` - Test programs

## Troubleshooting

### CMake Not Found
- Install CMake from https://cmake.org/download/
- Add CMake to your PATH
- Restart terminal after installation

### Compilation Errors
- Ensure you have a C++17 compatible compiler
- Check that all source files are present
- Verify include paths are correct

### Missing Dependencies
- SDL2 is optional (for GUI)
- All core functionality works without SDL2
- See `docs/SDL2_SETUP.md` for GUI setup

## Quick Test

After building, test with:
```powershell
.\build\bin\cinebrew.exe examples\hello.cb
```

This should compile `hello.cb` and produce bytecode.

