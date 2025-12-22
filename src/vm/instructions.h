/**
 * CINEBREW VM Instruction Set
 * 
 * ============================================================================
 * INSTRUCTION FORMAT
 * ============================================================================
 * 
 * Instructions are text-based for now (readable and debuggable).
 * Format: "OPCODE [OPERAND] [OPERAND]"
 * 
 * Examples:
 *   - "PUSH 5"        → Push the number 5
 *   - "ADD"            → Add top two stack values
 *   - "STORE x"       → Store top value in variable 'x'
 *   - "JMP loop"      → Jump to label 'loop'
 * 
 * ============================================================================
 * INSTRUCTION CATEGORIES
 * ============================================================================
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/**
 * STACK OPERATIONS
 * 
 * PUSH <value>    - Push a number onto the stack
 *                  Example: "PUSH 42" → stack: [42]
 * 
 * POP             - Remove top value (not implemented yet, use operations that pop)
 */

/**
 * ARITHMETIC OPERATIONS
 * 
 * ADD             - Pop two values, add them, push result
 *                  Stack: [a, b] → [a+b]
 * 
 * SUB             - Pop two values, subtract (b - a), push result
 *                  Stack: [a, b] → [b-a]
 * 
 * MUL             - Pop two values, multiply, push result
 *                  Stack: [a, b] → [a*b]
 * 
 * DIV             - Pop two values, divide (b / a), push result
 *                  Stack: [a, b] → [b/a]
 *                  WARNING: Division by zero not checked!
 */

/**
 * VARIABLE OPERATIONS
 * 
 * STORE <name>    - Pop value, store in variable
 *                  Example: "STORE x" → vars["x"] = top_value
 * 
 * LOAD <name>     - Load variable value onto stack
 *                  Example: "LOAD x" → push(vars["x"])
 */

/**
 * COMPARISON OPERATIONS
 * 
 * EQ              - Pop two values, push 1 if equal, else 0
 *                  Stack: [a, b] → [b==a ? 1 : 0]
 * 
 * GT              - Pop two values, push 1 if b > a, else 0
 *                  Stack: [a, b] → [b>a ? 1 : 0]
 * 
 * LT              - Pop two values, push 1 if b < a, else 0
 *                  Stack: [a, b] → [b<a ? 1 : 0]
 * 
 * NE              - Pop two values, push 1 if not equal, else 0
 *                  Stack: [a, b] → [b!=a ? 1 : 0]
 */

/**
 * CONTROL FLOW
 * 
 * JMP <label>     - Unconditional jump to label
 *                  Example: "JMP loop" → pc = labels["loop"]
 * 
 * JZ <label>      - Jump if zero (pop value, jump if == 0)
 *                  Example: "JZ end" → if (top == 0) goto end
 * 
 * JNZ <label>     - Jump if not zero (pop value, jump if != 0)
 *                  Example: "JNZ loop" → if (top != 0) goto loop
 * 
 * <label>:        - Label definition (ends with colon)
 *                  Example: "loop:" → labels["loop"] = current_pc
 */

/**
 * FUNCTION OPERATIONS
 * 
 * CALL <label> <argc>  - Call function with N arguments
 *                        Arguments must be on stack before CALL
 *                        Example: "CALL add 2" → calls add() with 2 args
 * 
 * LOADARG <n>     - Load function argument N (0-indexed)
 *                   Example: "LOADARG 0" → push first argument
 * 
 * RET             - Return from function
 *                   Pops return value, restores stack, returns to caller
 */

/**
 * I/O OPERATIONS
 * 
 * PRINT           - Print top stack value to console
 *                   Example: "PRINT" → cout << stack.top()
 * 
 * HALT            - Stop execution (label, not instruction)
 *                   Example: "HALT:" → program ends here
 */

#endif // INSTRUCTIONS_H

