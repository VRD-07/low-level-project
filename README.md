CineBrew

A custom programming language with a compiler, stack-based virtual machine, and runtime library, built from scratch.

Overview

CineBrew is an educational systems project that implements a complete language toolchain. It includes a lexer, recursive-descent parser, semantic analyzer, code generator, bytecode interpreter, and a small runtime library. The project demonstrates the core layers of a programming language: compilation to an intermediate representation and execution on a virtual machine.

The goal is to show how a simple imperative language can be implemented end-to-end without external compiler tools or language frameworks. CineBrew compiles to a text-based bytecode format that runs on a stack-based interpreter. The language supports variables, functions, control flow, and basic I/O.

What Was Built From Scratch

- Lexer: Tokenization with 31 token types and 11 keywords
- Parser: Recursive-descent parser constructing an abstract syntax tree (15 node types)
- Semantic analyzer: Symbol table, variable and function validation
- Code generator: AST to text-based bytecode
- Virtual machine: Stack-based interpreter with function frames
- Runtime: 14 built-in functions (I/O, math, timing, graphics stubs)
- Game loop: Frame-timed update/render pattern
- CLI interface: Single entry point for compilation and execution

Architecture Overview

The compiler pipeline takes source code, tokenizes it, parses into an AST, validates semantically, and generates bytecode. The bytecode is then executed by the VM:

  Source Code (.cb)
       ↓
    Lexer (tokenize)
       ↓
    Parser (AST)
       ↓
    Semantic (validate)
       ↓
    CodeGen (bytecode)
       ↓
    VM (execute)
       ↓
    Output (stdout)

Project Structure

- src/compiler: Lexer, parser, AST nodes, semantic analyzer, code generator
- src/vm: Stack-based virtual machine and instruction dispatch
- src/runtime: Built-in functions for I/O, math, and timing
- src/gui: Game loop and console-based window stub
- examples: Sample programs (hello.cb, factorial.cb, pong_game.cb)
- tests: 8 test suites covering lexer, parser, semantic, codegen, VM, and runtime
- docs: Language specification, bytecode reference, architecture notes

How to Build

Requirements: C++17 compiler, CMake 3.10+

Clone and build:

  git clone <repository>
  cd <repository>
  mkdir build && cd build
  cmake ..
  cmake --build .

Or on Windows with PowerShell:

  .\build.ps1

Executables are placed in build/bin/ after a successful build.

How to Run Programs

Run a CineBrew program:

  ./build/bin/cinebrew examples/hello.cb

Or using the helper script:

  ./run.sh examples/hello.cb          # Linux/macOS
  .\run.bat examples/hello.cb         # Windows

The output is text-based and printed to stdout.

Example Program

A simple program that prints a message:

  TAKE greeting
  POUR greeting = "Hello, World!"
  SHOT Print greeting

Variables are declared with TAKE, assigned with POUR, and functions are called with SHOT. The runtime Print function outputs to stdout.

Known Limitations

- GUI is a console-based stub. No graphics are rendered; drawing functions print debug output.
- Language features are intentionally minimal: integers, strings, functions, if/else, loops. No arrays, objects, or pointer semantics.
- Performance is not optimized. The bytecode interpreter is single-pass; no caching or JIT.
- Bytecode is text-based and not optimized for size or speed.
- No standard library beyond 14 built-in functions.
- Error messages are basic and may not always point to the exact line.

Hackathon Context

This project was built during a limited-time hackathon focused on systems fundamentals. The goal was to demonstrate understanding of compiler design and virtual machine implementation, not to build a production language. Time constraints meant focusing on correctness and architectural clarity over performance or feature completeness. Trade-offs were made intentionally: the GUI is stubbed, the language is small, and the bytecode format is simple and readable rather than optimized.
