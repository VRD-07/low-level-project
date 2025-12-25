

# 🚀 CineBrew — Custom Programming Language & Virtual Machine ☕  
*(Built From Scratch)*

CineBrew is an **educational systems-level project** that implements a complete programming language toolchain — from source code to execution — entirely from scratch.

It includes a **compiler**, a **stack-based virtual machine**, and a **minimal runtime**, designed to demonstrate how programming languages actually work under the hood.

---

## 📌 Overview

CineBrew is a small imperative programming language that compiles into a **text-based bytecode format**, which is then executed by a **stack-based virtual machine**.

The project showcases the full lifecycle of a language:

**Source Code → Compilation → Bytecode → Virtual Machine → Output**

The goal is not performance or feature completeness, but **clarity of architecture and correctness of fundamentals** — making it ideal as a learning and demonstration project.

---

## 🧠 What Was Built From Scratch

### Compiler Components
- **Lexer** — Tokenization with **31 token types** and **11 keywords**
- **Parser** — Recursive-descent parser producing an **AST (15 node types)**
- **Semantic Analyzer** — Symbol table, scope handling, and validation
- **Code Generator** — AST → text-based bytecode

### Runtime & Execution
- **Virtual Machine** — Stack-based interpreter with function frames
- **Runtime Library** — 14 built-in functions (I/O, math, timing, stubs)
- **Game Loop** — Frame-timed update/render pattern
- **CLI Interface** — Single entry point for compile + execute

---

## 🏗️ Architecture Overview

The CineBrew execution pipeline:

Source Code (.cb)
↓
Lexer (Tokenization)
↓
Parser (AST)
↓
Semantic Analysis
↓
Code Generation (Bytecode)
↓
Stack-Based Virtual Machine
↓
Output (stdout)


Each stage is explicitly implemented to keep the system **inspectable, debuggable, and educational**.

---

## 📁 Project Structure



src/

├── compiler/ # Lexer, parser, AST, semantic analyzer, code generator

├── vm/ # Stack-based virtual machine and instruction dispatch

├── runtime/ # Built-in functions (I/O, math, timing)

├── gui/ # Console-based game loop & window stub
examples/ # Sample programs (hello.cb, factorial.cb, pong_game.cb)
tests/ # 8 test suites covering all major subsystems
docs/ # Language spec, bytecode reference, architecture notes


---

## 🧩 Language Features

- Variables and assignments  
- Functions and function calls  
- Control flow (`if / else`, loops)  
- Basic input/output  
- Deterministic execution model  

### CineBrew keywords

```text
TAKE,       // Variable declaration: TAKE x = 5;
    POUR,       // Print statement: POUR x;
    SCENE,      // Function definition: SCENE add(a, b) { ... }
    SHOT,       // Return statement: SHOT result;
    IF,         // Conditional: IF x > 0 { ... }
    ELSE,       // Else clause: ELSE { ... }
    LOOP,       // While loop: LOOP i < 10 { ... }
    BREAK,      // Exit loop: BREAK;
    CONTINUE,   // Skip iteration: CONTINUE;
