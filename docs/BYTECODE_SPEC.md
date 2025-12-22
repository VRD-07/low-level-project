# CINEBREW Bytecode Specification
## STAGE 2 — BYTECODE DESIGN

---

## 📚 TABLE OF CONTENTS

1. [What is Bytecode?](#what-is-bytecode)
2. [Why Bytecode?](#why-bytecode)
3. [Instruction Format](#instruction-format)
4. [Operand Types](#operand-types)
5. [Instruction Encoding](#instruction-encoding)
6. [Text-Based Format (Current)](#text-based-format-current)
7. [Binary Format (Future)](#binary-format-future)
8. [Examples](#examples)

---

## WHAT IS BYTECODE?

**Bytecode** is an intermediate representation of code that sits between:
- **Source code** (human-readable: `x = 3 + 5`)
- **Machine code** (CPU-readable: `01010101...`)

### The Compilation Pipeline

```
CineBrew Source Code
    ↓ (Lexer)
Tokens
    ↓ (Parser)
AST (Abstract Syntax Tree)
    ↓ (Code Generator)
BYTECODE ← We are here!
    ↓ (VM Interpreter)
Execution & Results
```

### Analogy

Think of bytecode like **assembly language**:
- **Source code** = High-level language (like English)
- **Bytecode** = Assembly (like simplified instructions)
- **Machine code** = Binary (like 0s and 1s)

**Example:**
- **Source**: `x = 3 + 5`
- **Bytecode**: `PUSH 3`, `PUSH 5`, `ADD`, `STORE x`
- **Machine code**: `01010101 00110011...` (binary)

---

## WHY BYTECODE?

### 1. **Separation of Concerns**
- Compiler generates bytecode
- VM executes bytecode
- Can change compiler without changing VM (and vice versa)

### 2. **Portability**
- Same bytecode runs on any platform
- Only the VM needs to be ported
- Write once, run anywhere

### 3. **Simplicity**
- Easier to generate than machine code
- Easier to execute than parsing source
- Human-readable (for debugging)

### 4. **Flexibility**
- Can optimize bytecode before execution
- Can add new instructions easily
- Can analyze bytecode statically

### Real-World Examples

- **Java**: `.java` → `.class` (bytecode) → JVM
- **Python**: `.py` → `.pyc` (bytecode) → Python VM
- **C#**: `.cs` → IL (Intermediate Language) → .NET runtime

---

## INSTRUCTION FORMAT

### Current Format (Text-Based)

Each instruction is a **line of text** with this structure:

```
<OPCODE> [OPERAND1] [OPERAND2] ...
```

**Components:**
- **OPCODE**: The operation to perform (e.g., `PUSH`, `ADD`, `STORE`)
- **OPERANDS**: Zero or more arguments (space-separated)

### Examples

```
PUSH 42              # OPCODE: PUSH, OPERAND: 42
ADD                  # OPCODE: ADD, no operands
STORE x              # OPCODE: STORE, OPERAND: x
CALL add 2           # OPCODE: CALL, OPERANDS: add, 2
JMP loop             # OPCODE: JMP, OPERAND: loop
```

### Instruction Categories

#### 1. **Stack Operations**
- Format: `PUSH <number>`
- Example: `PUSH 42`

#### 2. **Arithmetic Operations**
- Format: `<OPCODE>` (no operands, uses stack)
- Examples: `ADD`, `SUB`, `MUL`, `DIV`

#### 3. **Variable Operations**
- Format: `<OPCODE> <identifier>`
- Examples: `STORE x`, `LOAD y`

#### 4. **Comparison Operations**
- Format: `<OPCODE>` (no operands, uses stack)
- Examples: `EQ`, `GT`, `LT`, `NE`

#### 5. **Control Flow**
- Format: `<OPCODE> <label>`
- Examples: `JMP loop`, `JZ end`, `JNZ body`
- Special: `<label>:` (label definition)

#### 6. **Function Operations**
- Format: `<OPCODE> <label> [arg_count]`
- Examples: `CALL add 2`, `LOADARG 0`, `RET`

#### 7. **I/O Operations**
- Format: `<OPCODE>` (no operands)
- Example: `PRINT`

---

## OPERAND TYPES

### 1. **Integer Literals**
- Format: Signed decimal number
- Examples: `42`, `-10`, `0`, `1000`
- Used in: `PUSH 42`

### 2. **Identifiers**
- Format: Alphanumeric + underscore, starting with letter/underscore
- Examples: `x`, `myVar`, `loop`, `add_function`
- Used in: `STORE x`, `LOAD myVar`, `JMP loop`, `CALL add`

### 3. **Labels**
- Format: Same as identifiers, but defined with `:`
- Examples: `loop:`, `end:`, `add:`
- Used in: `JMP loop`, `CALL add`

### 4. **Argument Index**
- Format: Non-negative integer
- Examples: `0`, `1`, `2`
- Used in: `LOADARG 0`, `LOADARG 1`

### Operand Rules

1. **Whitespace**: Operands separated by spaces
2. **Order Matters**: `CALL add 2` (label, then count)
3. **Case Sensitive**: `x` ≠ `X`
4. **No Quotes**: Identifiers don't need quotes (unlike strings)

---

## INSTRUCTION ENCODING

### Text-Based Encoding (Current)

**Format:** Plain text, one instruction per line

**Advantages:**
- ✅ Human-readable
- ✅ Easy to debug
- ✅ Easy to generate
- ✅ Easy to parse

**Disadvantages:**
- ❌ Larger file size
- ❌ Slower to parse
- ❌ Not optimized

**Example:**
```
PUSH 3
PUSH 5
ADD
STORE result
```

**Size:** ~30 bytes

---

### Binary Encoding (Future)

**Format:** Binary, compact representation

**Advantages:**
- ✅ Smaller file size
- ✅ Faster to load
- ✅ Can be optimized

**Disadvantages:**
- ❌ Not human-readable
- ❌ Harder to debug
- ❌ More complex to generate/parse

**How it would work:**

#### Opcode Encoding
Each opcode gets a number:

```
PUSH  = 0x01
ADD   = 0x02
SUB   = 0x03
MUL   = 0x04
DIV   = 0x05
STORE = 0x06
LOAD  = 0x07
...
```

#### Instruction Format

**Format 1: No Operands**
```
[OPCODE: 1 byte]
```

**Format 2: One Integer Operand**
```
[OPCODE: 1 byte] [VALUE: 4 bytes (int32)]
```

**Format 3: One String Operand**
```
[OPCODE: 1 byte] [LENGTH: 1 byte] [STRING: N bytes]
```

**Format 4: Two Operands**
```
[OPCODE: 1 byte] [OPERAND1: varies] [OPERAND2: varies]
```

#### Example Binary Encoding

**Text:**
```
PUSH 42
ADD
STORE x
```

**Binary (hypothetical):**
```
0x01 0x00 0x00 0x00 0x2A    # PUSH 42 (opcode + 4-byte int)
0x02                        # ADD (opcode only)
0x06 0x01 0x78             # STORE x (opcode + 1-byte length + 'x')
```

**Size:** ~10 bytes (vs ~20 bytes text)

---

## TEXT-BASED FORMAT (CURRENT)

### File Format

**Extension:** `.cbvm` (CineBrew Virtual Machine bytecode)

**Structure:**
- One instruction per line
- Empty lines ignored
- Comments: `#` (if we add them later)
- Labels: `<name>:`

### Example Bytecode File

```
# CineBrew Bytecode
# Example: Add two numbers

PUSH 10
STORE a
PUSH 20
STORE b
LOAD a
LOAD b
ADD
STORE result
LOAD result
PRINT
```

### Parsing Rules

1. **Trim whitespace** from start/end of lines
2. **Skip empty lines**
3. **Split by spaces** to get opcode and operands
4. **Convert operands** to appropriate types:
   - Try integer first
   - If fails, treat as identifier/label

### Error Handling

- **Unknown opcode**: Warning, skip instruction
- **Missing operand**: Error, skip instruction
- **Invalid operand**: Error, use default (0 or empty)

---

## BINARY FORMAT (FUTURE)

### Why Binary?

1. **Size**: 3-5x smaller than text
2. **Speed**: Faster to load and parse
3. **Security**: Harder to reverse-engineer (optional)

### Design Considerations

#### 1. **Opcode Table**

Create a mapping:
```cpp
enum class Opcode : uint8_t {
    PUSH = 0x01,
    ADD = 0x02,
    SUB = 0x03,
    MUL = 0x04,
    DIV = 0x05,
    STORE = 0x06,
    LOAD = 0x07,
    // ... etc
};
```

#### 2. **String Table**

Store all strings (identifiers, labels) in a table:
```
String Table:
  0: "x"
  1: "y"
  2: "loop"
  3: "add"
```

Then reference by index:
```
STORE 0    # Store to variable at index 0 ("x")
```

#### 3. **File Header**

```
Magic Number: "CBVM" (4 bytes)
Version: 1 (1 byte)
String Table Offset: (4 bytes)
Code Section Offset: (4 bytes)
```

#### 4. **Sections**

```
[Header: 13 bytes]
[String Table: variable size]
[Code Section: variable size]
```

### Conversion Process

**Text → Binary:**
1. Parse text bytecode
2. Build string table
3. Encode instructions
4. Write binary file

**Binary → Execution:**
1. Read header
2. Load string table
3. Decode instructions
4. Execute

---

## EXAMPLES

### Example 1: Simple Addition

**CineBrew Source (future):**
```cinebrew
x = 3 + 5
```

**Bytecode:**
```
PUSH 3
PUSH 5
ADD
STORE x
```

**Execution:**
1. Push 3 → Stack: `[3]`
2. Push 5 → Stack: `[3, 5]`
3. Add → Stack: `[8]`
4. Store x → Stack: `[]`, vars: `{x: 8}`

---

### Example 2: Variable Assignment

**CineBrew Source (future):**
```cinebrew
a = 10
b = 20
result = a + b
```

**Bytecode:**
```
PUSH 10
STORE a
PUSH 20
STORE b
LOAD a
LOAD b
ADD
STORE result
```

---

### Example 3: Loop

**CineBrew Source (future):**
```cinebrew
i = 5
while i > 0:
    i = i - 1
```

**Bytecode:**
```
PUSH 5
STORE i

loop:
LOAD i
PUSH 0
GT
JZ end

LOAD i
PUSH 1
SUB
STORE i
JMP loop

end:
```

---

### Example 4: Function Call

**CineBrew Source (future):**
```cinebrew
result = add(10, 20)
```

**Bytecode:**
```
PUSH 10
PUSH 20
CALL add 2
STORE result

add:
LOADARG 0
LOADARG 1
ADD
RET
```

---

## BYTECODE VALIDATION

### What to Check

1. **Opcode exists**: Is it a valid instruction?
2. **Operand count**: Does it match the opcode's requirements?
3. **Operand types**: Are operands the right type?
4. **Labels exist**: Are all referenced labels defined?
5. **Stack balance**: Does stack end up balanced? (advanced)

### Validation Rules

| Instruction | Required Operands | Types |
|------------|-------------------|-------|
| `PUSH` | 1 | integer |
| `ADD` | 0 | - |
| `STORE` | 1 | identifier |
| `LOAD` | 1 | identifier |
| `JMP` | 1 | label |
| `CALL` | 2 | label, integer |
| `LOADARG` | 1 | integer (0+) |
| `RET` | 0 | - |

---

## SUMMARY

### Current Implementation

- ✅ **Text-based bytecode**: Human-readable, easy to debug
- ✅ **One instruction per line**: Simple to parse
- ✅ **Space-separated operands**: Easy to split
- ✅ **Labels with `:`**: Clear label definitions

### Future Enhancements

- 🔄 **Binary format**: Smaller, faster (optional)
- 🔄 **Comments**: `#` for documentation
- 🔄 **Bytecode optimization**: Remove redundant instructions
- 🔄 **Validation**: Check bytecode before execution

### Key Takeaways

1. **Bytecode is intermediate code** between source and execution
2. **Text-based is perfect for now** - readable and debuggable
3. **Binary format is optional** - can add later if needed
4. **Format is simple** - one instruction per line, space-separated

---

## NEXT STEPS

**STAGE 3: LANGUAGE DESIGN**

Now that we understand bytecode, we'll design the CineBrew language syntax. The compiler will translate CineBrew source code into this bytecode format.

---

**END OF BYTECODE SPECIFICATION**

