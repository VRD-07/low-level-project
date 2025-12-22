#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT_DIR"
if [ ! -d build ]; then mkdir build; fi
cd build
if [ ! -f CMakeCache.txt ]; then cmake ..; fi
cmake --build .
cd "$ROOT_DIR"
# run cinebrew with passed args
if [ -x "build/bin/cinebrew" ]; then
  exec "build/bin/cinebrew" "$@"
else
  exec "build/bin/cinebrew.exe" "$@"
fi
