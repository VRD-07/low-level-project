/**
 * CINEBREW Virtual Machine - Implementation
 * 
 * This file implements a complete stack-based virtual machine.
 * Every function is documented to explain what it does and why.
 */

#include "vm.h"
#include <sstream>
#include <stdexcept>

// ============================================================================
// CONSTRUCTOR
// ============================================================================

VM::VM() {
    pc = 0;  // Start at instruction 0
    // Stack, vars, labels, callstack are automatically initialized
}

// ============================================================================
// STACK OPERATIONS
// ============================================================================

/**
 * Push a value onto the stack.
 * 
 * The stack is like a stack of plates - new items go on top.
 * 
 * Visual:
 *   Before: [10, 20]
 *   push(30)
 *   After:  [10, 20, 30]
 */
void VM::push(int value) {
    stack.push_back(value);
}

/**
 * Pop (remove and return) the top value from the stack.
 * 
 * Visual:
 *   Before: [10, 20, 30]
 *   pop() → returns 30
 *   After:  [10, 20]
 * 
 * WARNING: Popping from empty stack will crash!
 * (In a production system, we'd check for this)
 */
int VM::pop() {
    if (stack.empty()) {
        std::cerr << "ERROR: Attempted to pop from empty stack at PC=" << pc << std::endl;
        throw std::runtime_error("Stack underflow");
    }
    int value = stack.back();
    stack.pop_back();
    return value;
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * Split a string into words (space-separated).
 * 
 * Example: "PUSH 42" → ["PUSH", "42"]
 *          "ADD"      → ["ADD"]
 */
std::vector<std::string> VM::split(const std::string& s) {
    std::vector<std::string> parts;
    std::string cur;
    bool in_quote = false;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '"') {
            in_quote = !in_quote;
            // include quote in token so caller can strip if needed
            cur.push_back(c);
        } else if (std::isspace(static_cast<unsigned char>(c)) && !in_quote) {
            if (!cur.empty()) {
                parts.push_back(cur);
                cur.clear();
            }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) parts.push_back(cur);
    return parts;
}

// ============================================================================
// PREPROCESSING (Label Resolution)
// ============================================================================

/**
 * Preprocess the program to build the label map.
 * 
 * WHAT ARE LABELS?
 * Labels are named locations in the code. They allow us to jump
 * to specific places, like "goto" statements.
 * 
 * Example program:
 *   0: "PUSH 5"
 *   1: "loop:"      ← This is a label
 *   2: "PUSH 1"
 *   3: "ADD"
 *   4: "JMP loop"   ← Jump back to label "loop"
 * 
 * After preprocessing:
 *   labels["loop"] = 1
 * 
 * WHY DO THIS FIRST?
 * We need to know where labels are before we execute, because
 * JMP instructions reference them.
 */
void VM::preprocess(std::vector<std::string>& program) {
    labels.clear();
    
    for (int i = 0; i < (int)program.size(); i++) {
        std::string line = program[i];
        
        // Labels end with ':'
        if (!line.empty() && line.back() == ':') {
            // Extract label name (remove the ':')
            std::string label_name = line.substr(0, line.size() - 1);
            labels[label_name] = i;  // Map label name to instruction index
        }
    }
}

// ============================================================================
// INSTRUCTION EXECUTION
// ============================================================================

/**
 * Execute a single instruction.
 * 
 * This is the core of the VM - it reads an instruction string
 * and performs the corresponding operation.
 * 
 * HOW IT WORKS:
 *   1. Parse the instruction (split into parts)
 *   2. Identify the opcode (first word)
 *   3. Execute the appropriate operation
 *   4. Update the program counter (usually pc++)
 */
void VM::execute(const std::string& instruction, std::vector<std::string>& program) {
    // Skip empty lines
    if (instruction.empty()) {
        pc++;
        return;
    }
    
    // Skip label definitions (they're handled in preprocess)
    if (instruction.back() == ':') {
        pc++;
        return;
    }
    
    // Debug: show executing instruction
    std::cerr << "[DEBUG] PC=" << pc << " EXEC='" << instruction << "'" << std::endl;

    // Split instruction into parts: "PUSH 42" → ["PUSH", "42"]
    auto parts = split(instruction);
    if (parts.empty()) {
        pc++;
        return;
    }
    
    std::string opcode = parts[0];
    
    // ========================================================================
    // STACK OPERATIONS
    // ========================================================================
    
    if (opcode == "PUSH") {
        // PUSH <value> - Push a number onto the stack
        // Example: "PUSH 42" → push(42)
        if (parts.size() < 2) {
            std::cerr << "ERROR: PUSH requires a value at PC=" << pc << std::endl;
            pc++;
            return;
        }
        // Try to parse as integer, otherwise treat as string literal.
        try {
            int value = std::stoi(parts[1]);
            push(value);
            pc++;
        } catch (const std::invalid_argument&) {
            // Non-numeric push (likely a string). Extract the literal from
            // the original instruction (take everything after the opcode)
            std::string literal;
            size_t sep = instruction.find_first_of(" \t");
            if (sep != std::string::npos) {
                literal = instruction.substr(sep + 1);
                // Trim leading/trailing whitespace
                while (!literal.empty() && std::isspace(static_cast<unsigned char>(literal.front()))) literal.erase(literal.begin());
                while (!literal.empty() && std::isspace(static_cast<unsigned char>(literal.back()))) literal.pop_back();
            } else {
                literal = parts[1];
            }
            // strip surrounding quotes if present
            if (literal.size() >= 2 && literal.front() == '"' && literal.back() == '"') {
                literal = literal.substr(1, literal.size() - 2);
            }

            // Find next non-label instruction
            int next_i = pc + 1;
            while (next_i < (int)program.size() && !program[next_i].empty() && program[next_i].back() == ':') {
                next_i++;
            }

            if (next_i < (int)program.size()) {
                auto next_parts = split(program[next_i]);
                if (!next_parts.empty() && next_parts[0] == "PRINT") {
                    // Debug: show literal contents and length
                    std::cerr << "[DEBUG] Literal=('" << literal << "') len=" << literal.size() << std::endl;
                    // Print the literal directly
                    std::cout << literal << std::endl;
                    // Advance pc to after the PRINT
                    pc = next_i + 1;
                    return;
                }
            }

            // Fallback: push 0 and emit a warning
            std::cerr << "WARNING: PUSH of non-integer '" << parts[1] << "' at PC=" << pc << " - treating as 0" << std::endl;
            push(0);
            pc++;
        }
    }
    
    // ========================================================================
    // ARITHMETIC OPERATIONS
    // ========================================================================
    
    else if (opcode == "ADD") {
        // ADD - Pop two values, add them, push result
        // Stack: [a, b] → [a+b]
        // Note: We pop in reverse order (b first, then a)
        int b = pop();
        int a = pop();
        push(a + b);
        pc++;
    }
    
    else if (opcode == "SUB") {
        // SUB - Pop two values, subtract (a - b), push result
        // Stack: [a, b] → [a-b]
        int b = pop();
        int a = pop();
        push(a - b);
        pc++;
    }
    
    else if (opcode == "MUL") {
        // MUL - Pop two values, multiply, push result
        // Stack: [a, b] → [a*b]
        int b = pop();
        int a = pop();
        push(a * b);
        pc++;
    }
    
    else if (opcode == "DIV") {
        // DIV - Pop two values, divide (a / b), push result
        // Stack: [a, b] → [a/b]
        // WARNING: No division by zero check!
        int b = pop();
        int a = pop();
        if (b == 0) {
            std::cerr << "ERROR: Division by zero at PC=" << pc << std::endl;
            push(0);  // Return 0 on error
        } else {
            push(a / b);
        }
        pc++;
    }
    
    // ========================================================================
    // VARIABLE OPERATIONS
    // ========================================================================
    
    else if (opcode == "STORE") {
        // STORE <name> - Pop value, store in variable
        // Example: "STORE x" → vars["x"] = pop()
        if (parts.size() < 2) {
            std::cerr << "ERROR: STORE requires variable name at PC=" << pc << std::endl;
            pc++;
            return;
        }
        std::string var_name = parts[1];
        int value = pop();
        vars[var_name] = value;
        pc++;
    }
    
    else if (opcode == "LOAD") {
        // LOAD <name> - Load variable value onto stack
        // Example: "LOAD x" → push(vars["x"])
        if (parts.size() < 2) {
            std::cerr << "ERROR: LOAD requires variable name at PC=" << pc << std::endl;
            pc++;
            return;
        }
        std::string var_name = parts[1];
        
        // If variable doesn't exist, default to 0
        if (vars.find(var_name) == vars.end()) {
            std::cerr << "WARNING: Variable '" << var_name << "' not found, using 0 at PC=" << pc << std::endl;
            push(0);
        } else {
            push(vars[var_name]);
        }
        pc++;
    }
    
    // ========================================================================
    // COMPARISON OPERATIONS
    // ========================================================================
    
    else if (opcode == "EQ") {
        // EQ - Pop two values, push 1 if equal, else 0
        // Stack: [a, b] → [b==a ? 1 : 0]
        int b = pop();
        int a = pop();
        push(a == b ? 1 : 0);
        pc++;
    }
    
    else if (opcode == "GT") {
        // GT - Pop two values, push 1 if a > b, else 0
        // Stack: [a, b] → [a>b ? 1 : 0]
        int b = pop();
        int a = pop();
        push(a > b ? 1 : 0);
        pc++;
    }
    
    else if (opcode == "LT") {
        // LT - Pop two values, push 1 if a < b, else 0
        // Stack: [a, b] → [a<b ? 1 : 0]
        int b = pop();
        int a = pop();
        push(a < b ? 1 : 0);
        pc++;
    }
    
    else if (opcode == "NE") {
        // NE - Pop two values, push 1 if not equal, else 0
        // Stack: [a, b] → [a!=b ? 1 : 0]
        int b = pop();
        int a = pop();
        push(a != b ? 1 : 0);
        pc++;
    }
    
    // ========================================================================
    // CONTROL FLOW
    // ========================================================================
    
    else if (opcode == "JMP") {
        // JMP <label> - Unconditional jump to label
        // Example: "JMP loop" → pc = labels["loop"]
        if (parts.size() < 2) {
            std::cerr << "ERROR: JMP requires label name at PC=" << pc << std::endl;
            pc++;
            return;
        }
        std::string label = parts[1];
        
        if (labels.find(label) == labels.end()) {
            std::cerr << "ERROR: Label '" << label << "' not found at PC=" << pc << std::endl;
            pc++;
            return;
        }
        
        pc = labels[label];  // Jump to label (don't increment pc)
    }
    
    else if (opcode == "JZ") {
        // JZ <label> - Jump if zero
        // Pop value, if it's 0, jump to label; otherwise continue
        if (parts.size() < 2) {
            std::cerr << "ERROR: JZ requires label name at PC=" << pc << std::endl;
            pc++;
            return;
        }
        std::string label = parts[1];
        int value = pop();
        
        if (value == 0) {
            if (labels.find(label) == labels.end()) {
                std::cerr << "ERROR: Label '" << label << "' not found at PC=" << pc << std::endl;
                pc++;
            } else {
                pc = labels[label];
            }
        } else {
            pc++;  // Continue to next instruction
        }
    }
    
    else if (opcode == "JNZ") {
        // JNZ <label> - Jump if not zero
        // Pop value, if it's NOT 0, jump to label; otherwise continue
        if (parts.size() < 2) {
            std::cerr << "ERROR: JNZ requires label name at PC=" << pc << std::endl;
            pc++;
            return;
        }
        std::string label = parts[1];
        int value = pop();
        
        if (value != 0) {
            if (labels.find(label) == labels.end()) {
                std::cerr << "ERROR: Label '" << label << "' not found at PC=" << pc << std::endl;
                pc++;
            } else {
                pc = labels[label];
            }
        } else {
            pc++;  // Continue to next instruction
        }
    }
    
    // ========================================================================
    // FUNCTION OPERATIONS
    // ========================================================================
    
    else if (opcode == "CALL") {
        // CALL <label> <argc> - Call function with N arguments
        // 
        // HOW FUNCTION CALLS WORK:
        //   1. Arguments are already on the stack (pushed before CALL)
        //   2. We create a Frame to remember:
        //      - Where to return (return_pc = pc + 1)
        //      - How big the stack was before (prev_stack_size)
        //      - How many arguments (arg_count)
        //   3. Jump to the function label
        //   4. Function uses LOADARG to access arguments
        //   5. Function uses RET to return
        //
        // Example:
        //   PUSH 10
        //   PUSH 20
        //   CALL add 2    ← Call add() with 2 arguments
        //   PRINT
        //
        //   add:
        //   LOADARG 0     ← Get first argument (10)
        //   LOADARG 1     ← Get second argument (20)
        //   ADD
        //   RET           ← Return (result is on stack)
        
        if (parts.size() < 2) {
            std::cerr << "ERROR: CALL requires label name at PC=" << pc << std::endl;
            pc++;
            return;
        }
        
        std::string label = parts[1];
        int argc = 0;
        if (parts.size() >= 3) {
            argc = std::stoi(parts[2]);
        }
        
        // Calculate where arguments start on the stack
        int total_stack_size = (int)stack.size();
        int arg_start_index = total_stack_size - argc;
        
        // Create frame to remember call context
        Frame frame;
        frame.return_pc = pc + 1;        // Where to return to
        frame.prev_stack_size = arg_start_index;  // Stack size before arguments
        frame.arg_count = argc;          // Number of arguments
        
        // Check if this is a built-in function
        if (runtime.isBuiltin(label)) {
            // Built-in function: call runtime directly
            // Arguments are already on the stack
            std::vector<int> args;
            for (int i = 0; i < argc; i++) {
                int arg = pop();
                args.insert(args.begin(), arg);  // Insert at beginning to maintain order
            }
            
            // Call built-in function
            int result = runtime.call(label, args);
            
            // Push result onto stack
            push(result);
            
            pc++;
        } else {
            // User-defined function: use call stack
            callstack.push_back(frame);
            
            // Jump to function
            if (labels.find(label) == labels.end()) {
                std::cerr << "ERROR: Label '" << label << "' not found at PC=" << pc << std::endl;
                pc++;
            } else {
                pc = labels[label];
            }
        }
    }
    
    else if (opcode == "LOADARG") {
        // LOADARG <n> - Load function argument N (0-indexed)
        // 
        // HOW IT WORKS:
        //   - Arguments are on the stack before the function was called
        //   - We use the Frame to find where they are
        //   - Frame.prev_stack_size tells us where arguments start
        //   - We push the argument value onto the current stack
        //
        // Example: If add(10, 20) was called:
        //   - LOADARG 0 → pushes 10
        //   - LOADARG 1 → pushes 20
        
        if (parts.size() < 2) {
            std::cerr << "ERROR: LOADARG requires argument index at PC=" << pc << std::endl;
            push(0);
            pc++;
            return;
        }
        
        int arg_index = std::stoi(parts[1]);
        
        if (callstack.empty()) {
            // Not in a function call - return 0
            std::cerr << "WARNING: LOADARG called outside function at PC=" << pc << std::endl;
            push(0);
            pc++;
            return;
        }
        
        Frame& frame = callstack.back();
        
        // Check if argument index is valid
        if (arg_index < 0 || arg_index >= frame.arg_count) {
            std::cerr << "WARNING: Invalid argument index " << arg_index << " at PC=" << pc << std::endl;
            push(0);
            pc++;
            return;
        }
        
        // Calculate where this argument is in the stack
        // Arguments are stored at positions: prev_stack_size + arg_index
        // 
        // Example: If stack was [10, 20] and we CALL with 2 args:
        //   - prev_stack_size = 0 (arguments start at index 0)
        //   - LOADARG 0 → stack[0 + 0] = stack[0] = 10
        //   - LOADARG 1 → stack[0 + 1] = stack[1] = 20
        //
        // Even if the stack grows during function execution, the original
        // argument positions remain valid because RET will resize the stack
        // back to prev_stack_size, removing everything added during the function.
        
        int arg_position = frame.prev_stack_size + arg_index;
        
        if (arg_position < (int)stack.size()) {
            push(stack[arg_position]);
        } else {
            std::cerr << "WARNING: Argument position out of bounds at PC=" << pc << std::endl;
            push(0);
        }
        pc++;
    }
    
    else if (opcode == "RET") {
        // RET - Return from function
        // 
        // HOW IT WORKS:
        //   1. Pop the return value (if any) from stack
        //   2. Restore the stack to its size before the function call
        //   3. Push the return value back
        //   4. Restore the program counter from the frame
        //   5. Remove the frame from callstack
        //
        // Example:
        //   add:
        //   LOADARG 0
        //   LOADARG 1
        //   ADD        ← Result is on stack
        //   RET        ← Return with result
        
        if (callstack.empty()) {
            // No function to return from - stop execution
            pc = (int)program.size();
            return;
        }
        
        // Get return value (if stack has something)
        int return_value = 0;
        if (!stack.empty()) {
            return_value = pop();
        }
        
        // Get frame and remove it
        Frame frame = callstack.back();
        callstack.pop_back();
        
        // Restore stack to size before function call
        // Remove everything that was added during the function
        stack.resize(frame.prev_stack_size);
        
        // Push return value back
        push(return_value);
        
        // Return to caller
        pc = frame.return_pc;
    }
    
    // ========================================================================
    // I/O OPERATIONS
    // ========================================================================
    
    else if (opcode == "PRINT") {
        // PRINT - Print top stack value to console
        if (stack.empty()) {
            std::cout << "[EMPTY_STACK]" << std::endl;
        } else {
            std::cout << stack.back() << std::endl;
        }
        pc++;
    }
    
    // ========================================================================
    // UNKNOWN INSTRUCTION
    // ========================================================================
    
    else {
        std::cerr << "WARNING: Unknown instruction '" << opcode << "' at PC=" << pc << std::endl;
        pc++;  // Skip unknown instruction
    }
}

// ============================================================================
// PROGRAM EXECUTION
// ============================================================================

/**
 * Run a complete program.
 * 
 * HOW IT WORKS:
 *   1. Preprocess to build label map
 *   2. Reset VM state
 *   3. Execute instructions one by one until program ends
 * 
 * The program ends when:
 *   - pc >= program.size() (reached end)
 *   - RET is called with empty callstack (main function returned)
 */
void VM::run(std::vector<std::string> program) {
    // Step 1: Build label map
    preprocess(program);
    
    // Step 2: Reset VM state
    pc = 0;
    stack.clear();
    vars.clear();
    callstack.clear();
    
    // Step 3: Execute instructions
    while (pc < (int)program.size()) {
        execute(program[pc], program);
    }
}

// ============================================================================
// DEBUGGING UTILITIES
// ============================================================================

void VM::printStack() const {
    std::cout << "Stack: [";
    for (size_t i = 0; i < stack.size(); i++) {
        std::cout << stack[i];
        if (i < stack.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void VM::printVars() const {
    std::cout << "Variables:" << std::endl;
    for (const auto& pair : vars) {
        std::cout << "  " << pair.first << " = " << pair.second << std::endl;
    }
}

void VM::reset() {
    pc = 0;
    stack.clear();
    vars.clear();
    labels.clear();
    callstack.clear();
}

