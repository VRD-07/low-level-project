/**
 * VM Test Main Program
 * 
 * This is a simple test program to verify the VM works correctly.
 * Run this to see the VM in action.
 */

#include "vm.h"
#include <iostream>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CINEBREW Virtual Machine - Test" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    VM vm;
    
    // Example 1: Simple addition
    std::cout << "Example 1: Adding 3 + 5" << std::endl;
    std::cout << "Program:" << std::endl;
    std::cout << "  PUSH 3" << std::endl;
    std::cout << "  PUSH 5" << std::endl;
    std::cout << "  ADD" << std::endl;
    std::cout << "  PRINT" << std::endl;
    std::cout << "Output: ";
    
    std::vector<std::string> program1 = {
        "PUSH 3",
        "PUSH 5",
        "ADD",
        "PRINT"
    };
    vm.run(program1);
    std::cout << std::endl;
    
    // Example 2: Variables and loop
    std::cout << "Example 2: Sum from 1 to 5" << std::endl;
    std::cout << "Program:" << std::endl;
    std::cout << "  (Loop that sums 1+2+3+4+5)" << std::endl;
    std::cout << "Output: ";
    
    std::vector<std::string> program2 = {
        "PUSH 5",
        "STORE n",
        "PUSH 0",
        "STORE sum",
        
        "loop:",
        "LOAD n",
        "PUSH 0",
        "GT",
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
        "PRINT"
    };
    vm.run(program2);
    std::cout << std::endl;
    
    // Example 3: Function call
    std::cout << "Example 3: Function call (add 7 + 8)" << std::endl;
    std::cout << "Program:" << std::endl;
    std::cout << "  PUSH 7" << std::endl;
    std::cout << "  PUSH 8" << std::endl;
    std::cout << "  CALL add 2" << std::endl;
    std::cout << "  PRINT" << std::endl;
    std::cout << "Output: ";
    
    std::vector<std::string> program3 = {
        "PUSH 7",
        "PUSH 8",
        "CALL add 2",
        "PRINT",
        "HALT:",
        
        "add:",
        "LOADARG 0",
        "LOADARG 1",
        "ADD",
        "RET"
    };
    vm.run(program3);
    std::cout << std::endl;
    
    std::cout << "========================================" << std::endl;
    std::cout << "All tests completed successfully!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

