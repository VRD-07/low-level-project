/**
 * Parser Test Program
 * 
 * Tests the CineBrew parser with various source code examples.
 */

#include "../src/compiler/lexer.h"
#include "../src/compiler/parser.h"
#include "../src/compiler/ast.h"
#include <iostream>
#include <fstream>
#include <sstream>

void testParser(const std::string& source, const std::string& description) {
    std::cout << "\n=== " << description << " ===" << std::endl;
    std::cout << "Source code:" << std::endl;
    std::cout << source << std::endl;
    std::cout << std::endl;
    
    // Lex
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    if (lexer.hadError()) {
        std::cout << "Lexer Error: " << lexer.getError() << std::endl;
        return;
    }
    
    // Parse
    Parser parser(tokens);
    std::unique_ptr<Program> program = parser.parse();
    
    if (parser.hadError()) {
        std::cout << "Parser Error: " << parser.getError() << std::endl;
        return;
    }
    
    // Print AST
    std::cout << "AST:" << std::endl;
    std::cout << program->toString() << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CINEBREW Parser Test" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Test 1: Simple variable declaration
    testParser(
        "TAKE x = 5;",
        "Test 1: Simple Variable Declaration"
    );
    
    // Test 2: Arithmetic expression
    testParser(
        "TAKE result = 3 + 5;",
        "Test 2: Arithmetic Expression"
    );
    
    // Test 3: Complex expression
    testParser(
        "TAKE result = (3 + 5) * 2;",
        "Test 3: Complex Expression with Parentheses"
    );
    
    // Test 4: Conditional
    testParser(
        "IF x > 0 {\n    POUR x;\n}",
        "Test 4: Conditional Statement"
    );
    
    // Test 5: Conditional with ELSE
    testParser(
        "IF x > 0 {\n    POUR \"positive\";\n} ELSE {\n    POUR \"zero or negative\";\n}",
        "Test 5: Conditional with ELSE"
    );
    
    // Test 6: Loop
    testParser(
        "LOOP i < 5 {\n    POUR i;\n    i = i + 1;\n}",
        "Test 6: Loop Statement"
    );
    
    // Test 7: Function definition
    testParser(
        "SCENE add(a, b) {\n    SHOT a + b;\n}",
        "Test 7: Function Definition"
    );
    
    // Test 8: Function call
    testParser(
        "TAKE result = add(3, 5);",
        "Test 8: Function Call"
    );
    
    // Test 9: Comparison operators
    testParser(
        "TAKE eq = a == b;\nTAKE ne = a != b;\nTAKE gt = a > b;",
        "Test 9: Comparison Operators"
    );
    
    // Test 10: Complex program
    testParser(
        "TAKE x = 10;\n"
        "TAKE y = 20;\n"
        "IF x > 5 {\n"
        "    TAKE sum = x + y;\n"
        "    POUR sum;\n"
        "}",
        "Test 10: Complex Program"
    );
    
    // Test 11: Print statement
    testParser(
        "POUR 42;",
        "Test 11: Print Statement"
    );
    
    // Test 12: Boolean literals
    testParser(
        "TAKE flag = true;\nIF flag {\n    POUR \"true\";\n}",
        "Test 12: Boolean Literals"
    );
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

