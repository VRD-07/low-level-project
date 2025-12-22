# Virtual Machine Explanation
## CINEBREW VM - Complete Guide

---

## 📚 TABLE OF CONTENTS

1. [What is a Virtual Machine?](#what-is-a-virtual-machine)
2. [Why Use a VM?](#why-use-a-vm)
3. [How Our Stack-Based VM Works](#how-our-stack-based-vm-works)
4. [VM Components](#vm-components)
5. [Instruction Set](#instruction-set)
6. [Visual Examples](#visual-examples)
7. [Function Calls Explained](#function-calls-explained)

---

## WHAT IS A VIRTUAL MACHINE?

A **Virtual Machine (VM)** is a program that simulates a computer. Instead of running code directly on your CPU, the VM interprets instructions one by one.

### Real Computer vs Virtual Machine

**Real Computer:**
```
Source Code → Compiler → Machine Code (01010101...) → CPU executes directly
```

**Our VM:**
```
Source Code → Compiler → Bytecode (text instructions) → VM interprets → Results
```

### Analogy

Think of a VM like an **interpreter** for a foreign language:
- **Real CPU**: Speaks "machine code" natively
- **VM**: Speaks "bytecode" and translates it to actions your CPU understands

---

## WHY USE A VM?

### 1. **PORTABILITY** 🌍
- Write your program once
- Run it on Windows, Mac, Linux, etc.
- Only need to port the VM, not every program

**Example:** Java uses a VM (JVM) - that's why Java programs work everywhere!

### 2. **SIMPLICITY** 🎯
- Bytecode is human-readable
- Easy to debug (see exactly what's happening)
- Easier to understand than binary machine code

### 3. **SAFETY** 🛡️
- VM can check for errors before they crash
- Can prevent invalid operations
- Better error messages

### 4. **FLEXIBILITY** 🔧
- Want a new instruction? Just add it to the VM
- Easy to experiment with new features
- No need to change hardware

---

## HOW OUR STACK-BASED VM WORKS

### What is a Stack?

A **stack** is like a stack of plates:
- **PUSH**: Put a plate on top
- **POP**: Remove the top plate
- **LIFO**: Last In, First Out

```
Stack visualization:
    [30]  ← Top (last added)
    [20]
    [10]  ← Bottom (first added)
```

### Stack Operations

**PUSH:**
```
Before: [10, 20]
push(30)
After:  [10, 20, 30]
```

**POP:**
```
Before: [10, 20, 30]
pop() → returns 30
After:  [10, 20]
```

### Why Stack-Based?

Stack-based VMs are simple and elegant:
- Operations work on the top values
- No need to specify registers
- Easy to implement
- Used by: Python, Java (JVM), Lua, Forth

---

## VM COMPONENTS

Our VM has 5 main components:

### 1. **Stack** (`vector<int> stack`)
- Stores temporary values
- Used for arithmetic and function calls
- Grows and shrinks as program runs

### 2. **Variables** (`unordered_map<string, int> vars`)
- Stores named variables
- Example: `vars["x"] = 42`
- Global scope (all functions can access)

### 3. **Program Counter** (`int pc`)
- Points to current instruction
- Starts at 0
- Increments after each instruction (unless jumping)

### 4. **Labels** (`unordered_map<string, int> labels`)
- Maps label names to instruction indices
- Used for jumps and function calls
- Example: `labels["loop"] = 5`

### 5. **Call Stack** (`vector<Frame> callstack`)
- Tracks function calls
- Each Frame remembers:
  - Where to return (`return_pc`)
  - Stack size before call (`prev_stack_size`)
  - Number of arguments (`arg_count`)

---

## INSTRUCTION SET

### Stack Operations

| Instruction | Description | Example |
|------------|-------------|---------|
| `PUSH <n>` | Push number onto stack | `PUSH 42` |

### Arithmetic

| Instruction | Description | Stack Effect |
|------------|-------------|--------------|
| `ADD` | Add top two values | `[a, b] → [a+b]` |
| `SUB` | Subtract (a - b) | `[a, b] → [a-b]` |
| `MUL` | Multiply | `[a, b] → [a*b]` |
| `DIV` | Divide (a / b) | `[a, b] → [a/b]` |

### Variables

| Instruction | Description | Example |
|------------|-------------|---------|
| `STORE <name>` | Store top value in variable | `STORE x` |
| `LOAD <name>` | Load variable onto stack | `LOAD x` |

### Comparisons

| Instruction | Description | Result |
|------------|-------------|--------|
| `EQ` | Equal? | `[a, b] → [a==b ? 1 : 0]` |
| `GT` | Greater than? | `[a, b] → [a>b ? 1 : 0]` |
| `LT` | Less than? | `[a, b] → [a<b ? 1 : 0]` |
| `NE` | Not equal? | `[a, b] → [a!=b ? 1 : 0]` |

### Control Flow

| Instruction | Description | Example |
|------------|-------------|---------|
| `JMP <label>` | Unconditional jump | `JMP loop` |
| `JZ <label>` | Jump if zero | `JZ end` |
| `JNZ <label>` | Jump if not zero | `JNZ loop` |
| `<label>:` | Define label | `loop:` |

### Functions

| Instruction | Description | Example |
|------------|-------------|---------|
| `CALL <label> <n>` | Call function with n args | `CALL add 2` |
| `LOADARG <n>` | Load argument n | `LOADARG 0` |
| `RET` | Return from function | `RET` |

### I/O

| Instruction | Description |
|------------|-------------|
| `PRINT` | Print top stack value |

---

## VISUAL EXAMPLES

### Example 1: Adding 3 + 5

**Program:**
```
PUSH 3
PUSH 5
ADD
PRINT
```

**Execution:**

| Step | Instruction | Stack | PC |
|------|-------------|-------|-----|
| 0 | `PUSH 3` | `[3]` | 1 |
| 1 | `PUSH 5` | `[3, 5]` | 2 |
| 2 | `ADD` | `[8]` | 3 |
| 3 | `PRINT` | `[8]` | 4 |
| | Output: `8` | | |

**Explanation:**
1. Push 3 → stack: `[3]`
2. Push 5 → stack: `[3, 5]`
3. ADD pops 5 and 3, adds them (3+5=8), pushes 8 → stack: `[8]`
4. PRINT outputs 8

---

### Example 2: Variables

**Program:**
```
PUSH 10
STORE x
PUSH 20
STORE y
LOAD x
LOAD y
ADD
PRINT
```

**Execution:**

| Step | Instruction | Stack | Variables | PC |
|------|-------------|-------|-----------|-----|
| 0 | `PUSH 10` | `[10]` | `{}` | 1 |
| 1 | `STORE x` | `[]` | `{x: 10}` | 2 |
| 2 | `PUSH 20` | `[20]` | `{x: 10}` | 3 |
| 3 | `STORE y` | `[]` | `{x: 10, y: 20}` | 4 |
| 4 | `LOAD x` | `[10]` | `{x: 10, y: 20}` | 5 |
| 5 | `LOAD y` | `[10, 20]` | `{x: 10, y: 20}` | 6 |
| 6 | `ADD` | `[30]` | `{x: 10, y: 20}` | 7 |
| 7 | `PRINT` | `[30]` | `{x: 10, y: 20}` | 8 |
| | Output: `30` | | | |

---

### Example 3: Loop (Sum 1 to 3)

**Program:**
```
PUSH 3
STORE n
PUSH 0
STORE sum

loop:
LOAD n
PUSH 0
GT
JNZ body
JMP end

body:
LOAD sum
LOAD n
ADD
STORE sum

LOAD n
PUSH 1
SUB
STORE n
JMP loop

end:
LOAD sum
PRINT
```

**Execution Flow:**

```
Initial: n=3, sum=0

Iteration 1:
  n > 0? Yes → body
  sum = 0 + 3 = 3
  n = 3 - 1 = 2
  → loop

Iteration 2:
  n > 0? Yes → body
  sum = 3 + 2 = 5
  n = 2 - 1 = 1
  → loop

Iteration 3:
  n > 0? Yes → body
  sum = 5 + 1 = 6
  n = 1 - 1 = 0
  → loop

Iteration 4:
  n > 0? No → end
  PRINT sum → 6
```

**Output:** `6` (1+2+3)

---

## FUNCTION CALLS EXPLAINED

Function calls are the most complex part. Let's break it down:

### How Function Calls Work

**Step 1: Prepare Arguments**
```
PUSH 10
PUSH 20
```
Stack: `[10, 20]`

**Step 2: Call Function**
```
CALL add 2
```
- Creates a Frame:
  - `return_pc = 4` (next instruction after CALL)
  - `prev_stack_size = 0` (arguments start at index 0)
  - `arg_count = 2`
- Pushes frame onto callstack
- Jumps to `add:` label

**Step 3: Function Execution**
```
add:
LOADARG 0    // Push argument 0 (10)
LOADARG 1    // Push argument 1 (20)
ADD          // 10 + 20 = 30
RET          // Return with 30 on stack
```

**Step 4: Return**
- RET pops return value (30)
- Restores stack to size before call (removes function's stack additions)
- Pushes return value back
- Jumps to `return_pc` (instruction 4)

**Step 5: Continue**
```
PRINT        // Prints 30
```

### Visual: Function Call Stack

```
Before CALL:
  Stack: [10, 20]
  Callstack: []

During CALL (at add function):
  Stack: [10, 20, 10, 20, 30]  (after LOADARG and ADD)
  Callstack: [Frame{return_pc=4, prev_stack_size=0, arg_count=2}]

After RET:
  Stack: [30]
  Callstack: []
  PC = 4 (back to caller)
```

### Why This Design?

1. **Arguments stay on stack**: Simple, no copying needed
2. **Frame tracks context**: Knows where to return and how to restore stack
3. **RET cleans up**: Removes function's stack additions automatically

---

## SUMMARY

Our VM is a **stack-based interpreter** that:
- Executes text-based bytecode instructions
- Uses a stack for temporary values
- Supports variables, control flow, and functions
- Is portable, simple, and safe

**Next Steps:**
- In STAGE 2, we'll design the bytecode format
- In STAGE 3, we'll design the CineBrew language
- In STAGE 7, we'll generate bytecode from CineBrew source

---

**END OF VM EXPLANATION**

