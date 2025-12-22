CineBrew — packaged

This repository contains the CineBrew language, compiler, runtime and VM packaged for easy building and running.

Build (cross-platform, requires CMake & a C++17 toolchain):

  mkdir build && cd build
  cmake ..
  cmake --build .

Or on Windows with PowerShell:

  .\build.ps1

Run:

  ./build/bin/cinebrew examples/hello.cb
  cinebrew run examples/hello.cb    # equivalent

Helper scripts:

  run.sh   # Linux/macOS: builds if needed and runs cinebrew with args
  run.bat  # Windows: builds if needed and runs cinebrew with args

How to write a program:

- Create a text file with `.cb` extension using CineBrew language constructs.
- See `examples/hello.cb` and `examples/factorial.cb` for minimal programs.

Known limitations:

- GUI is a console/backend stub; no SDL graphics by default.
- Language is intentionally small (integers, strings, functions, control flow).
- No command-line flags except `run` shorthand. Keep programs simple.

This package does not change core compiler, VM, or runtime logic — only provides a single, documented entrypoint, simple build rules and run scripts for convenience.
