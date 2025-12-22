/**
 * Factorial Test Program
 * 
 * Computes factorial of 5 using the VM.
 * Demonstrates loops and variables.
 */

#include "../src/vm/vm.h"
#include <iostream>

int main() {
    std::cout << "=== CINEBREW VM - Factorial Test ===" << std::endl;
    std::cout << "Computing factorial of 5..." << std::endl;
    std::cout << std::endl;
    
    VM vm;
    
    // Factorial program: result = 1; i = 5; while i > 0: result *= i; i--
    std::vector<std::string> program = {
        "PUSH 5",
        "STORE n",
        
        "PUSH 1",
        "STORE result",
        
        "LOAD n",
        "STORE i",
        
        "loop:",
        "LOAD i",
        "PUSH 0",
        "GT",           // i > 0?
        "JNZ body",
        "JMP end",
        
        "body:",
        "LOAD result",
        "LOAD i",
        "MUL",
        "STORE result",
        
        "LOAD i",
        "PUSH 1",
        "SUB",
        "STORE i",
        "JMP loop",
        
        "end:",
        "LOAD result",
        "PRINT"  // Should print 120 (5! = 5*4*3*2*1)
    };
    
    vm.run(program);
    
    std::cout << std::endl;
    std::cout << "Expected: 120 (5! = 5*4*3*2*1)" << std::endl;
    
    return 0;
}

