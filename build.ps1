# CINEBREW Build Script
# This script helps build the CINEBREW project

Write-Host "CINEBREW Build Script" -ForegroundColor Cyan
Write-Host "====================" -ForegroundColor Cyan
Write-Host ""

# Check for CMake
$cmakeFound = $false
try {
    $cmakeVersion = cmake --version 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host "CMake found:" -ForegroundColor Green
        Write-Host $cmakeVersion -ForegroundColor Green
        $cmakeFound = $true
    }
} catch {
    Write-Host "CMake not found in PATH" -ForegroundColor Yellow
}

if (-not $cmakeFound) {
    Write-Host ""
    Write-Host "CMake is required to build this project." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "To install CMake:" -ForegroundColor Cyan
    Write-Host "  1. Download from: https://cmake.org/download/" -ForegroundColor White
    Write-Host "  2. Or use: winget install cmake" -ForegroundColor White
    Write-Host "  3. Or use: choco install cmake" -ForegroundColor White
    Write-Host ""
    Write-Host "After installing CMake, restart your terminal and run this script again." -ForegroundColor Yellow
    exit 1
}

# Check for C++ compiler
Write-Host "Checking for C++ compiler..." -ForegroundColor Cyan
$compilerFound = $false

# Check for g++
try {
    $gppVersion = g++ --version 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Found g++:" -ForegroundColor Green
        Write-Host $gppVersion -ForegroundColor Green
        $compilerFound = $true
        $generator = "MinGW Makefiles"
    }
} catch {
    # Check for cl (Visual Studio)
    try {
        $clVersion = cl 2>&1
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Found Visual Studio compiler" -ForegroundColor Green
            $compilerFound = $true
            $generator = "Visual Studio 17 2022"
        }
    } catch {
        Write-Host "No C++ compiler found" -ForegroundColor Yellow
    }
}

if (-not $compilerFound) {
    Write-Host ""
    Write-Host "C++ compiler not found. Please install:" -ForegroundColor Yellow
    Write-Host "  - MinGW-w64 (for g++)" -ForegroundColor White
    Write-Host "  - Visual Studio (for cl)" -ForegroundColor White
    exit 1
}

# Create build directory
Write-Host ""
Write-Host "Creating build directory..." -ForegroundColor Cyan
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
    Write-Host "Created build directory" -ForegroundColor Green
} else {
    Write-Host "Build directory already exists" -ForegroundColor Green
}

# Configure CMake
Write-Host ""
Write-Host "Configuring CMake..." -ForegroundColor Cyan
Set-Location build

# Try to help CMake find SDL2 by checking common locations or env vars
$cmakeArgs = @()

# Prefer explicit SDL2_DIR environment variable if provided
if ($env:SDL2_DIR) {
    Write-Host "Using SDL2_DIR from environment: $env:SDL2_DIR" -ForegroundColor Green
    $cmakeArgs += "-DSDL2_DIR=$env:SDL2_DIR"
} else {
    # Common locations to probe for SDL2Config.cmake
    $probePaths = @("C:/SDL2/lib/cmake/SDL2", "C:/vcpkg/installed/x64-windows/share/SDL2", "/mingw64/lib/cmake/SDL2", "/usr/local/lib/cmake/SDL2")
    foreach ($p in $probePaths) {
        # Convert unix-style paths on Windows when present
        $pWindows = $p -replace '/','\\'
        if (Test-Path $p) {
            Write-Host "Found SDL2 at: $p" -ForegroundColor Green
            $cmakeArgs += "-DSDL2_DIR=$p"
            break
        } elseif (Test-Path $pWindows) {
            Write-Host "Found SDL2 at: $pWindows" -ForegroundColor Green
            $cmakeArgs += "-DSDL2_DIR=$pWindows"
            break
        }
    }
}

# If CMAKE_PREFIX_PATH environment variable is set, pass it along
if ($env:CMAKE_PREFIX_PATH) {
    Write-Host "Using CMAKE_PREFIX_PATH from environment: $env:CMAKE_PREFIX_PATH" -ForegroundColor Green
    $cmakeArgs += "-DCMAKE_PREFIX_PATH=$env:CMAKE_PREFIX_PATH"
}

if ($generator -eq "MinGW Makefiles") {
    $args = @('..', '-G', 'MinGW Makefiles') + $cmakeArgs
    & cmake @args
} else {
    $args = @('..') + $cmakeArgs
    & cmake @args
}

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Build
Write-Host ""
Write-Host "Building project..." -ForegroundColor Cyan
cmake --build .

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host ""
Write-Host "Build successful!" -ForegroundColor Green
Write-Host ""
Write-Host "Executables are in: build\bin\" -ForegroundColor Cyan
Write-Host ""
Write-Host "To run the compiler (from repository root):" -ForegroundColor Cyan
Write-Host "  .\build\bin\cinebrew examples\hello.cb" -ForegroundColor White
Write-Host ""
Write-Host "To run a game (from repository root):" -ForegroundColor Cyan
Write-Host "  .\build\bin\cinebrew examples\pong_game.cb" -ForegroundColor White

Set-Location ..

