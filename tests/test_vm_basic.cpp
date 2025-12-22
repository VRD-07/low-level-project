/**
 * Basic VM Test Program
 * 
 * This demonstrates the core VM functionality:
 * - Stack operations
 * - Arithmetic
 * - Variables
 * - Control flow
 */

#include "../src/vm/vm.h"
#include <iostream>

int main() {
    std::cout << "=== CINEBREW VM - Basic Test ===" << std::endl;
    std::cout << std::endl;
    
    VM vm;
    
    // Test 1: Simple arithmetic
    std::cout << "Test 1: Simple Arithmetic (3 + 5)" << std::endl;
    std::vector<std::string> program1 = {
        "PUSH 3",
        "PUSH 5",
        "ADD",
        "PRINT"  // Should print 8
    };
    vm.run(program1);
    std::cout << std::endl;
    
    // Test 2: Variables
    std::cout << "Test 2: Variables" << std::endl;
    std::vector<std::string> program2 = {
        "PUSH 10",
        "STORE x",
        "PUSH 20",
        "STORE y",
        "LOAD x",
        "LOAD y",
        "ADD",
        "PRINT"  // Should print 30
    };
    vm.run(program2);
    std::cout << std::endl;
    
    // Test 3: Control flow (loop)
    std::cout << "Test 3: Control Flow (Sum 1 to 5)" << std::endl;
    std::vector<std::string> program3 = {
        "PUSH 5",
        "STORE n",
        "PUSH 0",
        "STORE sum",
        
        "loop:",
        "LOAD n",
        "PUSH 0",
        "GT",           // n > 0?
        "JNZ body",
        "JMP end",
        
        "body:",
        "LOAD sum",
        "LOAD n",
        "ADD",
        "STORE sum",
        
        "LOAD n",
        "PUSH 1",
        "SUB",
        "STORE n",
        "JMP loop",
        
        "end:",
        "LOAD sum",
        "PRINT"  // Should print 15 (1+2+3+4+5)
    };
    vm.run(program3);
    std::cout << std::endl;
    
    // Test 4: Function call
    std::cout << "Test 4: Function Call (add function)" << std::endl;
    std::vector<std::string> program4 = {
        "PUSH 7",
        "PUSH 8",
        "CALL add 2",
        "PRINT",  // Should print 15
        "HALT:",
        
        "add:",
        "LOADARG 0",  // First argument (7)
        "LOADARG 1",  // Second argument (8)
        "ADD",
        "RET"
    };
    vm.run(program4);
    std::cout << std::endl;
    
    std::cout << "=== All Tests Complete ===" << std::endl;
    
    return 0;
}

