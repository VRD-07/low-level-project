CineBrew

A small programming language implemented from scratch, featuring a compiler, a stack-based virtual machine, and a runtime environment.

Overview

CineBrew is a systems-level programming language project that implements a complete language toolchain end to end. It includes lexical analysis, parsing, semantic validation, bytecode generation, and execution on a custom-built virtual machine.

The project is intentionally minimal and explicit. Instead of relying on external compiler frameworks or VM libraries, every major component is implemented manually to expose how programming languages actually work beneath the surface. Source code is compiled into a readable, text-based bytecode format and executed on a stack-based interpreter.

CineBrew is designed as an educational and experimental system. It prioritizes clarity of architecture and correctness of execution over performance or feature breadth.

What Was Built From Scratch

Lexer
Manual tokenization with 31 token types and 11 language keywords.

Parser
Recursive-descent parser producing an abstract syntax tree (15 node types).

Semantic Analyzer
Symbol tables and validation for variables and functions.

Code Generator
AST to text-based bytecode transformation.

Virtual Machine
Stack-based bytecode interpreter with function frames and control flow.

Runtime System
14 built-in functions for I/O, math, timing, and graphics stubs.

Game Loop
Frame-timed update/render structure (console-backed).

Command-Line Interface
Single executable for compilation and execution.

Architecture Overview

CineBrew follows a classic compiler + VM pipeline. Each stage is explicit and independently inspectable.

Source Code (.cb)
        ↓
      Lexer
        ↓
      Parser
        ↓
 Semantic Analysis
        ↓
  Bytecode Generator
        ↓
  Stack-Based VM
        ↓
      Output


The bytecode format is human-readable by design, making execution behavior easy to trace and debug.

Project Structure

src/compiler/
Lexer, parser, AST nodes, semantic analysis, and code generation.

src/vm/
Virtual machine core and instruction dispatch loop.

src/runtime/
Built-in functions and shared runtime utilities.

src/gui/
Console-based game loop and rendering stubs.

examples/
Sample programs (hello, factorial, pong logic).

tests/
Test suites covering lexer, parser, semantic analysis, code generation, VM, and runtime.

docs/
Language specification, bytecode reference, and architecture notes.

How to Build

Requirements

C++17 compatible compiler

CMake 3.10+

Build from a fresh clone:

git clone <repo-url>
cd cinebrew
mkdir build && cd build
cmake ..
cmake --build .


On Windows (PowerShell):

.\build.ps1


The executable is generated in build/bin/.

How to Run Programs

Run a CineBrew source file directly:

./build/bin/cinebrew examples/hello.cb


Or using helper scripts:

./run.sh examples/hello.cb     # Linux / macOS
.\run.bat examples\hello.cb    # Windows


Program output is printed to standard output.

Example Program

A minimal CineBrew program:

TAKE greeting
POUR greeting = "Hello, World!"
SHOT Print greeting


TAKE declares a variable

POUR assigns a value

SHOT invokes a function

The Print function is provided by the runtime and writes to stdout.

Known Limitations

The GUI is a console-backed stub. No actual graphics are rendered; draw calls emit debug output.

The language is intentionally small: integers, strings, functions, conditionals, and loops.
There are no arrays, objects, or pointers.

The virtual machine is not optimized. There is no bytecode caching, JIT compilation, or parallel execution.

Bytecode is text-based and favors readability over compactness or speed.

Error reporting is basic and may not always point to exact source locations.

The runtime library is minimal and limited to 14 built-in functions.

Hackathon Context

CineBrew was developed during a limited-time hackathon focused on building systems from scratch. The intent was not to create a production-ready language, but to demonstrate a clear understanding of compiler construction, virtual machine design, and runtime execution.

Given time constraints, design decisions favored architectural transparency and correctness over performance and feature completeness. The GUI remains stubbed, the language remains small, and the bytecode format remains readable — all intentional trade-offs to keep the system understandable and inspectable.
