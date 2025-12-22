# CINEBREW Language Specification
## STAGE 3 — LANGUAGE DESIGN

---

## 📚 TABLE OF CONTENTS

1. [Language Overview](#language-overview)
2. [Design Philosophy](#design-philosophy)
3. [Lexical Structure](#lexical-structure)
4. [Keywords](#keywords)
5. [Data Types](#data-types)
6. [Variables](#variables)
7. [Expressions](#expressions)
8. [Statements](#statements)
9. [Control Flow](#control-flow)
10. [Functions](#functions)
11. [Grammar Rules](#grammar-rules)
12. [Examples](#examples)
13. [Language to Bytecode Mapping](#language-to-bytecode-mapping)

---

## LANGUAGE OVERVIEW

**CINEBREW** is a programming language designed with a **cinema + coffee** theme, making it both fun and memorable. It's a simple, imperative language perfect for learning programming concepts and game development.

### Language Characteristics

- **Type**: Imperative, procedural
- **Paradigm**: Structured programming
- **Case Sensitivity**: Yes (case-sensitive)
- **Typing**: Dynamic (integers only for now)
- **Scope**: Global variables, local function parameters

### Design Goals

1. **Educational**: Easy to learn and understand
2. **Themed**: Fun keywords (cinema + coffee)
3. **Game-Ready**: Built-in support for game concepts
4. **Simple**: Minimal syntax, maximum clarity

---

## DESIGN PHILOSOPHY

### Why Cinema + Coffee?

- **Cinema**: Represents storytelling, scenes, shots, takes
- **Coffee**: Represents brewing, pouring, taking
- **Combined**: Creates a memorable, thematic language

### Syntax Principles

1. **Traditional Symbols**: Use familiar `;`, `()`, `{}`, `=`
2. **Readable**: Code should read like English
3. **Consistent**: Similar constructs use similar syntax
4. **Minimal**: No unnecessary complexity

---

## LEXICAL STRUCTURE

### Character Set

- **Letters**: `a-z`, `A-Z`
- **Digits**: `0-9`
- **Whitespace**: Space, tab, newline
- **Symbols**: `;`, `(`, `)`, `{`, `}`, `=`, `+`, `-`, `*`, `/`, `==`, `!=`, `>`, `<`, `>=`, `<=`

### Comments

```
# This is a single-line comment
# Comments start with # and continue to end of line
```

### Identifiers

- **Rules**:
  - Start with letter or underscore
  - Followed by letters, digits, or underscores
  - Case-sensitive
- **Examples**: `x`, `myVar`, `player_score`, `_temp`

### Literals

- **Integers**: `42`, `-10`, `0`, `1000`
- **Strings**: `"hello"`, `"world"` (for future use)
- **Booleans**: `true`, `false` (keywords, not literals yet)

---

## KEYWORDS

### Core Keywords (Cinema + Coffee Theme)

| Keyword | Meaning | Usage |
|---------|---------|-------|
| `TAKE` | Variable declaration | `TAKE x = 5;` |
| `POUR` | Print/output | `POUR x;` |
| `SCENE` | Function definition | `SCENE add(a, b) { ... }` |
| `SHOT` | Return statement | `SHOT result;` |
| `IF` | Conditional | `IF condition { ... }` |
| `ELSE` | Else clause | `ELSE { ... }` |
| `LOOP` | While loop | `LOOP condition { ... }` |
| `BREAK` | Exit loop | `BREAK;` |
| `CONTINUE` | Skip to next iteration | `CONTINUE;` |
| `true` | Boolean true | `IF true { ... }` |
| `false` | Boolean false | `IF false { ... }` |

### Keyword Meanings

- **TAKE**: "Take" a variable (declare/assign)
- **POUR**: "Pour" output (print)
- **SCENE**: A "scene" is a function (like a movie scene)
- **SHOT**: "Shot" returns from a scene (like a camera shot)
- **LOOP**: Loop (no theme, but clear)

---

## DATA TYPES

### Current Types

1. **Integer**: `42`, `-10`, `0`
   - 32-bit signed integers
   - All arithmetic operations supported

2. **Boolean**: `true`, `false`
   - Result of comparisons
   - Used in conditionals

### Future Types (Not Implemented Yet)

- **String**: `"hello"`
- **Float**: `3.14`
- **Array**: `[1, 2, 3]`

---

## VARIABLES

### Declaration and Assignment

**Syntax:**
```
TAKE <identifier> = <expression>;
```

**Examples:**
```cinebrew
TAKE x = 5;
TAKE y = 10;
TAKE result = x + y;
```

### Reassignment

Variables can be reassigned without `TAKE`:
```cinebrew
TAKE x = 5;
x = 10;        # Reassign x to 10
x = x + 1;     # Increment x
```

### Variable Rules

- **Global Scope**: All variables are global (for now)
- **No Type Declaration**: Type inferred from value
- **Must Initialize**: Variables must be assigned when declared
- **Case Sensitive**: `x` ≠ `X`

---

## EXPRESSIONS

### Arithmetic Expressions

**Operators:**
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division

**Examples:**
```cinebrew
TAKE sum = 3 + 5;           # 8
TAKE diff = 10 - 3;         # 7
TAKE product = 4 * 5;       # 20
TAKE quotient = 20 / 4;     # 5
TAKE complex = (3 + 5) * 2;  # 16
```

### Comparison Expressions

**Operators:**
- `==` Equal
- `!=` Not equal
- `>` Greater than
- `<` Less than
- `>=` Greater than or equal
- `<=` Less than or equal

**Examples:**
```cinebrew
TAKE isEqual = 5 == 5;      # true
TAKE isGreater = 10 > 5;    # true
TAKE isLess = 3 < 2;        # false
```

### Operator Precedence

1. Parentheses: `()`
2. Multiplication/Division: `*`, `/` (left to right)
3. Addition/Subtraction: `+`, `-` (left to right)
4. Comparisons: `==`, `!=`, `>`, `<`, `>=`, `<=`
5. Assignment: `=`

---

## STATEMENTS

### Expression Statement

Any expression followed by `;`:
```cinebrew
x + y;        # Valid but result is discarded
x = 5;        # Assignment statement
```

### Block Statement

Statements grouped with `{ }`:
```cinebrew
{
    TAKE x = 5;
    TAKE y = 10;
    POUR x + y;
}
```

---

## CONTROL FLOW

### IF Statement

**Syntax:**
```
IF <condition> {
    <statements>
}
```

**With ELSE:**
```
IF <condition> {
    <statements>
} ELSE {
    <statements>
}
```

**Examples:**
```cinebrew
TAKE x = 10;

IF x > 5 {
    POUR "x is greater than 5";
} ELSE {
    POUR "x is 5 or less";
}
```

### LOOP Statement (While Loop)

**Syntax:**
```
LOOP <condition> {
    <statements>
}
```

**Examples:**
```cinebrew
TAKE i = 0;
LOOP i < 5 {
    POUR i;
    i = i + 1;
}
```

### BREAK and CONTINUE

**BREAK**: Exit loop immediately
```cinebrew
LOOP true {
    TAKE x = input();
    IF x == 0 {
        BREAK;
    }
    POUR x;
}
```

**CONTINUE**: Skip to next iteration
```cinebrew
TAKE i = 0;
LOOP i < 10 {
    i = i + 1;
    IF i % 2 == 0 {
        CONTINUE;  # Skip even numbers
    }
    POUR i;
}
```

---

## FUNCTIONS

### Function Definition

**Syntax:**
```
SCENE <name>(<param1>, <param2>, ...) {
    <statements>
    SHOT <expression>;  # Optional, returns 0 if omitted
}
```

**Examples:**
```cinebrew
SCENE add(a, b) {
    SHOT a + b;
}

SCENE square(x) {
    SHOT x * x;
}

SCENE greet() {
    POUR "Hello!";
    SHOT 0;
}
```

### Function Call

**Syntax:**
```
<name>(<arg1>, <arg2>, ...)
```

**Examples:**
```cinebrew
TAKE result = add(3, 5);      # result = 8
TAKE squared = square(4);      # squared = 16
greet();                       # Call function, discard result
```

### Function Rules

- **Parameters**: Passed by value
- **Return Value**: Use `SHOT` to return
- **No Return**: Returns 0 if no `SHOT`
- **Recursion**: Supported (functions can call themselves)

---

## GRAMMAR RULES

### Formal Grammar (BNF-like)

```
Program     ::= Statement*

Statement   ::= Declaration
             |  Assignment
             |  ExpressionStmt
             |  IfStmt
             |  LoopStmt
             |  BreakStmt
             |  ContinueStmt
             |  ReturnStmt
             |  PrintStmt
             |  Block

Declaration ::= "TAKE" Identifier "=" Expression ";"

Assignment  ::= Identifier "=" Expression ";"

ExpressionStmt ::= Expression ";"

IfStmt      ::= "IF" Expression Block ["ELSE" Block]

LoopStmt    ::= "LOOP" Expression Block

BreakStmt   ::= "BREAK" ";"

ContinueStmt ::= "CONTINUE" ";"

ReturnStmt  ::= "SHOT" Expression ";"

PrintStmt   ::= "POUR" Expression ";"

Block       ::= "{" Statement* "}"

Expression  ::= AssignmentExpr
             |  ComparisonExpr
             |  AdditiveExpr

AssignmentExpr ::= Identifier "=" Expression
                 |  ComparisonExpr

ComparisonExpr ::= AdditiveExpr (("==" | "!=" | ">" | "<" | ">=" | "<=") AdditiveExpr)?

AdditiveExpr ::= MultiplicativeExpr (("+" | "-") MultiplicativeExpr)*

MultiplicativeExpr ::= UnaryExpr (("*" | "/") UnaryExpr)*

UnaryExpr   ::= PrimaryExpr
             |  "-" PrimaryExpr

PrimaryExpr ::= Literal
             |  Identifier
             |  FunctionCall
             |  "(" Expression ")"

FunctionCall ::= Identifier "(" (Expression ("," Expression)*)? ")"

FunctionDef ::= "SCENE" Identifier "(" (Identifier ("," Identifier)*)? ")" Block

Literal     ::= Integer
             |  "true"
             |  "false"

Identifier  ::= Letter (Letter | Digit | "_")*

Integer     ::= ("-")? Digit+
```

---

## EXAMPLES

### Example 1: Hello World

```cinebrew
POUR "Hello, World!";
```

### Example 2: Variables and Arithmetic

```cinebrew
TAKE a = 10;
TAKE b = 20;
TAKE sum = a + b;
POUR sum;
```

### Example 3: Conditional

```cinebrew
TAKE x = 15;

IF x > 10 {
    POUR "x is greater than 10";
} ELSE {
    POUR "x is 10 or less";
}
```

### Example 4: Loop

```cinebrew
TAKE i = 1;
LOOP i <= 5 {
    POUR i;
    i = i + 1;
}
```

### Example 5: Function

```cinebrew
SCENE add(a, b) {
    SHOT a + b;
}

TAKE result = add(3, 5);
POUR result;
```

### Example 6: Factorial

```cinebrew
SCENE factorial(n) {
    IF n <= 1 {
        SHOT 1;
    }
    
    SHOT n * factorial(n - 1);
}

TAKE result = factorial(5);
POUR result;
```

### Example 7: Complex Program

```cinebrew
SCENE square(x) {
    SHOT x * x;
}

SCENE isEven(n) {
    SHOT n % 2 == 0;
}

TAKE num = 10;
TAKE squared = square(num);
POUR squared;

IF isEven(squared) {
    POUR "The square is even";
} ELSE {
    POUR "The square is odd";
}
```

---

## LANGUAGE TO BYTECODE MAPPING

### How CineBrew Translates to Bytecode

#### Variable Declaration
```cinebrew
TAKE x = 5;
```
**Bytecode:**
```
PUSH 5
STORE x
```

#### Arithmetic Expression
```cinebrew
TAKE result = a + b;
```
**Bytecode:**
```
LOAD a
LOAD b
ADD
STORE result
```

#### Conditional (IF)
```cinebrew
IF x > 0 {
    POUR x;
}
```
**Bytecode:**
```
LOAD x
PUSH 0
GT
JZ end_if
LOAD x
PRINT
end_if:
```

#### Loop (LOOP)
```cinebrew
LOOP i < 5 {
    i = i + 1;
}
```
**Bytecode:**
```
loop_start:
LOAD i
PUSH 5
LT
JZ loop_end
LOAD i
PUSH 1
ADD
STORE i
JMP loop_start
loop_end:
```

#### Function Definition
```cinebrew
SCENE add(a, b) {
    SHOT a + b;
}
```
**Bytecode:**
```
add:
LOADARG 0
LOADARG 1
ADD
RET
```

#### Function Call
```cinebrew
TAKE result = add(3, 5);
```
**Bytecode:**
```
PUSH 3
PUSH 5
CALL add 2
STORE result
```

#### Print Statement
```cinebrew
POUR x;
```
**Bytecode:**
```
LOAD x
PRINT
```

---

## DESIGN DECISIONS

### Why These Keywords?

1. **TAKE**: "Take" a variable - intuitive for assignment
2. **POUR**: "Pour" output - visual and memorable
3. **SCENE**: Functions are like movie scenes - self-contained
4. **SHOT**: Return is like a camera shot - captures the result

### Why Traditional Symbols?

- **Familiarity**: Programmers know `;`, `{}`, `()`
- **Clarity**: No ambiguity
- **Standard**: Matches most languages

### Why Case-Sensitive?

- **Consistency**: Matches C/C++/Java style
- **Flexibility**: `x` and `X` can be different
- **Standard**: Most modern languages are case-sensitive

### Why Semicolons?

- **Clarity**: Explicit statement boundaries
- **Parsing**: Easier to parse
- **Standard**: Matches C-style languages

---

## SUMMARY

### Language Features

✅ **Variables**: `TAKE x = 5;`  
✅ **Expressions**: Arithmetic, comparisons  
✅ **Control Flow**: `IF`, `ELSE`, `LOOP`, `BREAK`, `CONTINUE`  
✅ **Functions**: `SCENE`, `SHOT`  
✅ **I/O**: `POUR`  
✅ **Comments**: `#`  

### Syntax Characteristics

- **Themed Keywords**: Cinema + coffee
- **Traditional Symbols**: `;`, `{}`, `()`, `=`
- **Case-Sensitive**: Yes
- **Semicolons**: Required
- **Blocks**: `{ }` for grouping

### Next Steps

**STAGE 4: LEXER**

We'll build a lexer that tokenizes CineBrew source code according to this specification.

---

**END OF LANGUAGE SPECIFICATION**

