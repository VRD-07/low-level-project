You are a senior software engineer writing a README.md
for a “Build From Scratch” hackathon project.

This is NOT a marketing document.
This is a technical, honest, judge-facing README.

The project is a custom programming language with:
- a compiler
- a virtual machine
- a runtime
- an experimental GUI (not production-ready)

DO NOT invent features.
DO NOT exaggerate capabilities.
DO NOT use hype words.
DO NOT sound like an AI.

────────────────────────
README REQUIREMENTS
────────────────────────

Write a SINGLE README.md with the following sections ONLY,
in this exact order:

────────────────────────
1. Project Title + One-Line Description
────────────────────────
- Simple, professional title
- One sentence describing what it is

────────────────────────
2. Overview
────────────────────────
- 2–3 short paragraphs
- Explain:
  - Why this project exists
  - What was built from scratch
  - What problem it solves (learning / systems focus is fine)

────────────────────────
3. What Was Built From Scratch
────────────────────────
Bullet list, explicit and honest.
Examples:
- Lexer & parser
- Bytecode / IR
- Virtual machine
- Runtime system
- CLI interface

No fluff.

────────────────────────
4. Architecture Overview
────────────────────────
- Short explanation of the execution flow
- Include a SIMPLE ASCII diagram, for example:

  Source Code (.cb)
        ↓
     Compiler
        ↓
     Bytecode
        ↓
        VM
        ↓
     Output

Keep it minimal.

────────────────────────
5. Project Structure
────────────────────────
Explain each top-level folder in 1 line:
- compiler/
- vm/
- runtime/
- gui/
- examples/

────────────────────────
6. How to Build
────────────────────────
Exact commands.
Assume a fresh clone.
No assumptions about IDEs.

────────────────────────
7. How to Run Programs
────────────────────────
Show:
- How to run a sample program
- Example command
- Expected type of output (text-based)

────────────────────────
8. Example Program
────────────────────────
Show a VERY small example of the language
(5–10 lines max).

────────────────────────
9. Known Limitations
────────────────────────
Be honest.
Mention:
- GUI is experimental / stubbed
- Performance is not optimized
- Language features are limited by design

This section is CRITICAL for credibility.

────────────────────────
10. Hackathon Context
────────────────────────
One short paragraph:
- Built during a limited-time hackathon
- Focused on systems fundamentals
- Trade-offs were intentional

────────────────────────
STYLE RULES
────────────────────────
- Clear, technical, human-written
- Slightly imperfect is OK
- No emojis
- No hype words (revolutionary, cutting-edge, etc.)
- No AI voice

Generate ONLY the README.md content.
Do not explain your choices.
Do not add extra sections.

Proceed.
