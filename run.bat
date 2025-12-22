@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
cd /d %~dp0
if not exist build mkdir build
cd build
if not exist CMakeCache.txt cmake ..
cmake --build .
cd ..
if exist build\bin\cinebrew.exe (
  build\bin\cinebrew.exe %*
) else (
  if exist build\bin\cinebrew (
    build\bin\cinebrew %*
  ) else (
    echo Executable not found in build\bin\
    exit /b 1
  )
)
