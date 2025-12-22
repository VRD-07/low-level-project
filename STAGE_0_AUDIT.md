# STAGE 0 — PROJECT AUDIT
## CINEBREW Language & Runtime System

**Date:** Current Audit  
**Purpose:** Understand existing codebase, identify gaps, and plan complete rebuild

---

## 📋 EXECUTIVE SUMMARY

**Current State:** Partial implementation with multiple incomplete components  
**Status:** Foundation exists but needs consolidation and completion  
**Critical Gaps:** Parser, AST, Code Generator, GUI integration, Game examples

---

## 🔍 EXISTING FILES ANALYSIS

### 1. **Virtual Machine Files**

#### `vm.cpp` (Basic VM - ~196 lines)
**What it does:**
- Implements a stack-based virtual machine
- Supports basic arithmetic (ADD, SUB, MUL, DIV)
- Variable storage/loading (STORE, LOAD)
- Control flow (JMP, JZ, JNZ)
- Comparison operations (EQ, GT, LT)
- Simple function calls (CALL, RET) with basic callstack
- Label preprocessing
- PRINT instruction

**Issues Found:**
- ❌ Simple callstack (just stores return PC, no frame management)
- ❌ No argument passing mechanism
- ❌ No LOADARG instruction
- ❌ String parsing uses `substr()` which is fragile
- ❌ No error handling for empty stack
- ❌ Missing while loop in `run()` method (line 156-159 is incomplete)
- ⚠️ Global variable storage (no scoping)

**Strengths:**
- ✅ Core stack operations work
- ✅ Label system functional
- ✅ Basic control flow implemented

---

#### `vm_stage4.cpp` (Advanced VM - ~329 lines)
**What it does:**
- More sophisticated VM with Frame-based function calls
- Proper argument handling (LOADARG instruction)
- Frame structure tracks: return_pc, prev_stack_size, arg_count
- Better instruction parsing (uses string splitting)
- Multiple example programs (add, square, factorial)

**Issues Found:**
- ❌ Still text-based bytecode (not binary)
- ❌ No GUI integration
- ❌ No runtime library functions
- ⚠️ Multiple VM files (needs consolidation)

**Strengths:**
- ✅ Proper function call semantics
- ✅ Frame-based stack management
- ✅ Good example programs
- ✅ Better error handling

**Recommendation:** Use `vm_stage4.cpp` as the base, but consolidate into a single `vm.cpp` file.

---

### 2. **Lexer Files**

#### `cinebrew/lexer.h` (Header - Empty)
**Status:** File exists but is empty (should contain declarations)

---

#### `cinebrew/lexer.cpp` (Implementation - ~198 lines)
**What it does:**
- Defines TokenType enum with CineBrew keywords:
  - `TAKE`, `POUR`, `SCENE`, `SHOT`, `IF`, `ELSE`, `LOOP`, `TRUE_KW`, `FALSE_KW`
- Defines Token structure
- Implements basic Lexer struct with:
  - Source code storage
  - Token list
  - Position tracking (start, current, line)

**Issues Found:**
- ❌ **CRITICAL:** Syntax errors:
  - Line 123: Uses `LEFT_PAREN` but enum has `LPAREN`
  - Line 126: Uses `RIGHT_PAREN` but enum has `RPAREN`
  - Line 129: Uses `LEFT_BRACE` but enum has `LBRACE`
  - Line 132: Uses `RIGHT_BRACE` but enum has `RBRACE`
- ❌ **CRITICAL:** Duplicate `isDigit()` function (lines 162-165 and 167-170)
- ❌ Missing `stringLiteral()` function (called on line 140)
- ❌ Missing `identifier()` function (called on line 151)
- ❌ Missing keyword lookup table
- ❌ No handling for operators (PLUS, MINUS, STAR, SLASH)
- ❌ Main function is empty
- ❌ No tokenization loop

**Strengths:**
- ✅ Good token type design
- ✅ Position tracking infrastructure
- ✅ Basic character classification functions

**Recommendation:** Complete rewrite needed, but keep the token design.

---

#### `cinebrew/token.h` (Header - Empty)
**Status:** Should contain Token and TokenType definitions (currently in lexer.cpp)

---

#### `cinebrew/main.cpp` (Entry Point - Empty)
**Status:** Should be the compiler entry point, but is empty

---

### 3. **Other Files**

#### `vm_stage5.cpp` (Empty)
**Status:** File exists but is empty

#### `tempCodeRunnerFile.cpp` (Temporary)
**Status:** Contains only one line: `auto program = program_add_example();`  
**Recommendation:** Delete this file

---

## 🏗️ MISSING COMPONENTS

### Critical Missing Pieces:

1. **Parser** ❌
   - No AST construction
   - No syntax tree representation
   - No recursive descent parser

2. **AST (Abstract Syntax Tree)** ❌
   - No node types defined
   - No tree structure

3. **Code Generator** ❌
   - No translation from AST → bytecode
   - No compiler backend

4. **Language Grammar** ❌
   - No formal grammar definition
   - No language specification document

5. **Semantic Analyzer** ❌
   - No variable existence checking
   - No function call validation
   - No type checking (if needed)

6. **Runtime Library** ❌
   - No built-in functions (print, input, etc.)
   - No game-related functions

7. **GUI Integration** ❌
   - No graphics library (SDL/SFML)
   - No window management
   - No input handling
   - No rendering functions

8. **Game Loop** ❌
   - No update/render cycle
   - No frame timing

9. **Build System** ❌
   - No CMakeLists.txt
   - No Makefile
   - No build instructions

10. **Documentation** ❌
    - No README
    - No language specification
    - No usage examples

11. **Test Programs** ❌
    - No CineBrew source files (.cb)
    - No game examples

---

## 📁 PROPOSED DIRECTORY STRUCTURE

```
VM/
├── README.md                    # Project overview
├── CMakeLists.txt              # Build configuration
│
├── src/                        # Source code
│   ├── vm/                     # Virtual Machine
│   │   ├── vm.h
│   │   ├── vm.cpp
│   │   └── instructions.h      # Instruction definitions
│   │
│   ├── compiler/               # Compiler components
│   │   ├── lexer.h
│   │   ├── lexer.cpp
│   │   ├── token.h
│   │   ├── parser.h
│   │   ├── parser.cpp
│   │   ├── ast.h               # AST node definitions
│   │   ├── ast.cpp
│   │   ├── codegen.h           # Code generator
│   │   ├── codegen.cpp
│   │   └── semantic.h          # Semantic analyzer
│   │
│   ├── runtime/                # Runtime library
│   │   ├── runtime.h
│   │   └── runtime.cpp         # Built-in functions
│   │
│   ├── gui/                    # GUI integration
│   │   ├── window.h
│   │   ├── window.cpp
│   │   └── game_loop.h
│   │
│   └── main.cpp                # Compiler entry point
│
├── examples/                   # Example CineBrew programs
│   ├── hello.cb
│   ├── calculator.cb
│   └── game_pong.cb            # The final game
│
├── tests/                      # Test programs
│   └── test_vm.cpp
│
└── docs/                       # Documentation
    ├── LANGUAGE_SPEC.md        # Language grammar
    ├── BYTECODE_SPEC.md        # Bytecode format
    └── ARCHITECTURE.md         # System architecture
```

---

## 🐛 BUGS IDENTIFIED

### Critical Bugs:

1. **lexer.cpp:123-132** - Token type mismatches
   - `LEFT_PAREN` → should be `LPAREN`
   - `RIGHT_PAREN` → should be `RPAREN`
   - `LEFT_BRACE` → should be `LBRACE`
   - `RIGHT_BRACE` → should be `RBRACE`

2. **lexer.cpp:162-170** - Duplicate `isDigit()` function definition

3. **lexer.cpp:140** - Missing `stringLiteral()` function

4. **lexer.cpp:151** - Missing `identifier()` function

5. **vm.cpp:156-159** - Incomplete while loop (missing condition)

### Design Issues:

1. **Multiple VM implementations** - Need to consolidate
2. **No separation of concerns** - Everything mixed in single files
3. **No error reporting system** - Just exits on errors
4. **No file I/O** - Programs hardcoded in main()

---

## ✅ WHAT WORKS

1. **VM Core Stack Operations** - Push/pop work correctly
2. **VM Arithmetic** - ADD, SUB, MUL, DIV functional
3. **VM Variables** - STORE/LOAD work
4. **VM Control Flow** - JMP, JZ, JNZ work
5. **VM Labels** - Label preprocessing works
6. **VM Function Calls** - Basic CALL/RET work (in vm_stage4.cpp)
7. **Token Type Design** - Good enum structure
8. **Lexer Infrastructure** - Position tracking, character classification

---

## 🎯 RECOMMENDATIONS

### Immediate Actions:

1. **Consolidate VM files** → Single `vm.cpp` based on `vm_stage4.cpp`
2. **Fix lexer.cpp** → Complete implementation, fix syntax errors
3. **Separate token.h** → Move Token/TokenType to separate header
4. **Create build system** → CMakeLists.txt
5. **Delete temp files** → Remove `tempCodeRunnerFile.cpp`, `vm_stage5.cpp` (if empty)

### Architecture Decisions Needed:

1. **Bytecode Format:**
   - Keep text-based for now (readable, debuggable)
   - Can add binary format later

2. **Graphics Library:**
   - Recommend SDL2 (cross-platform, simple)
   - Alternative: SFML (more C++-like)

3. **Error Handling:**
   - Create ErrorReporter class
   - Collect all errors before exiting
   - Provide line numbers and context

4. **File Structure:**
   - Single executable: `cinebrew` (compiler + VM)
   - Or separate: `cinebrew-compile` and `cinebrew-run`

---

## 📊 COMPLETION STATUS

| Component | Status | Completion |
|-----------|--------|------------|
| VM Core | ✅ Partial | 70% |
| VM Functions | ✅ Partial | 60% |
| Lexer | ❌ Broken | 30% |
| Parser | ❌ Missing | 0% |
| AST | ❌ Missing | 0% |
| Code Generator | ❌ Missing | 0% |
| Semantic Analysis | ❌ Missing | 0% |
| Runtime Library | ❌ Missing | 0% |
| GUI Integration | ❌ Missing | 0% |
| Game Loop | ❌ Missing | 0% |
| Build System | ❌ Missing | 0% |
| Documentation | ❌ Missing | 0% |
| **OVERALL** | **❌ Incomplete** | **~15%** |

---

## 🚀 NEXT STEPS (STAGE 1)

After this audit is approved, we will:

1. **Clean up existing files**
   - Fix lexer.cpp bugs
   - Consolidate VM files
   - Organize directory structure

2. **Build VM Foundation (STAGE 1)**
   - Finalize VM implementation
   - Add missing instructions
   - Create comprehensive VM documentation
   - Write VM tests

3. **Proceed through remaining stages** in order

---

## 📝 NOTES

- The project has a solid foundation but needs significant work
- The VM is the most complete component
- The lexer needs immediate fixes before proceeding
- No parser means no language compilation yet
- The themed keywords (TAKE, POUR, SCENE, SHOT) are a good start

---

**END OF AUDIT**

*Ready to proceed to STAGE 1 after approval.*

