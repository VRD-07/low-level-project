/**
 * Runtime Library Test Program
 * 
 * Tests built-in functions in the runtime library.
 */

#include "../src/compiler/compiler.h"
#include "../src/vm/vm.h"
#include <iostream>
#include <sstream>

void testRuntimeFunction(const std::string& source, const std::string& description) {
    std::cout << "\n=== " << description << " ===" << std::endl;
    std::cout << "Source code:" << std::endl;
    std::cout << source << std::endl;
    std::cout << std::endl;
    
    // Compile
    Compiler compiler;
    std::vector<std::string> bytecode = compiler.compile(source);
    
    if (compiler.hadError()) {
        std::cout << "Compilation Errors:" << std::endl;
        for (const auto& error : compiler.getErrors()) {
            std::cout << "  " << error << std::endl;
        }
        return;
    }
    
    std::cout << "Execution:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    VM vm;
    vm.run(bytecode);
    std::cout << "----------------------------------------" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CINEBREW Runtime Library Test" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Test 1: print function
    testRuntimeFunction(
        "TAKE x = 42;\n"
        "POUR x;",
        "Test 1: Print (using POUR statement)"
    );
    
    // Test 2: random function
    testRuntimeFunction(
        "TAKE r = random(10);\n"
        "POUR r;",
        "Test 2: Random Number (0-9)"
    );
    
    // Test 3: abs function
    testRuntimeFunction(
        "TAKE neg = -10;\n"
        "TAKE pos = abs(neg);\n"
        "POUR pos;",
        "Test 3: Absolute Value"
    );
    
    // Test 4: min function
    testRuntimeFunction(
        "TAKE result = min(5, 10);\n"
        "POUR result;",
        "Test 4: Minimum Value"
    );
    
    // Test 5: max function
    testRuntimeFunction(
        "TAKE result = max(5, 10);\n"
        "POUR result;",
        "Test 5: Maximum Value"
    );
    
    // Test 6: time function
    testRuntimeFunction(
        "TAKE t = time();\n"
        "POUR t;",
        "Test 6: Current Time"
    );
    
    // Test 7: Complex program with multiple built-ins
    testRuntimeFunction(
        "TAKE a = 15;\n"
        "TAKE b = 8;\n"
        "TAKE minVal = min(a, b);\n"
        "TAKE maxVal = max(a, b);\n"
        "TAKE diff = maxVal - minVal;\n"
        "POUR \"Difference: \";\n"
        "POUR diff;",
        "Test 7: Multiple Built-in Functions"
    );
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

