# CineBrew SDL2 Build Guide for Windows (MSVC)

## Prerequisites
- CMake 3.10+
- MSVC compiler (Visual Studio 2019 or later)
- vcpkg installed at `C:\vcpkg`

## Step 1: Install SDL2 Dependencies via vcpkg

```powershell
cd C:\vcpkg
.\vcpkg install sdl2:x64-windows
.\vcpkg install sdl2-gfx:x64-windows
```

## Step 2: Clean Previous Build (if any)

```powershell
cd C:\Users\Acer\Desktop\Project\VM
if (Test-Path build) { Remove-Item -Recurse -Force build }
mkdir build
cd build
```

## Step 3: Configure CMake with vcpkg Toolchain

```powershell
cd C:\Users\Acer\Desktop\Project\VM\build
cmake -G "Visual Studio 17 2022" `
  -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows `
  ..
```

**Alternative for Visual Studio 2019:**
```powershell
cmake -G "Visual Studio 16 2019" `
  -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows `
  ..
```

## Step 4: Build the Project

```powershell
cd C:\Users\Acer\Desktop\Project\VM\build
cmake --build . --config Release
```

## Step 5: Run the Game

```powershell
cd C:\Users\Acer\Desktop\Project\VM\build
.\Release\game_runner.exe ..\examples\pong_game.cb
```

## Confirmation Criteria
✅ A window titled "CineBrew Game" opens  
✅ Window is responsive (can move/close it)  
✅ Drawing commands produce visual output  
✅ Pressing arrow keys changes paddle position  

## Troubleshooting

### CMake Configuration Fails
- Ensure vcpkg is at `C:\vcpkg`
- Run: `cd C:\vcpkg && git pull && .\vcpkg update`

### SDL2 not found
```powershell
C:\vcpkg\vcpkg install sdl2:x64-windows --recurse
C:\vcpkg\vcpkg install sdl2-gfx:x64-windows --recurse
```

### Linker Errors
- Verify: `C:\vcpkg\installed\x64-windows\lib\SDL2.lib` exists
- Rebuild: `cmake --build . --config Release --verbose`

### Window appears but nothing draws
- Check console output for "[GUI]" messages
- Verify renderer_ is non-null in window.cpp

