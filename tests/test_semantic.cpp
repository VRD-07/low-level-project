/**
 * Semantic Analyzer Test Program
 * 
 * Tests the semantic analyzer with various programs.
 */

#include "../src/compiler/lexer.h"
#include "../src/compiler/parser.h"
#include "../src/compiler/semantic.h"
#include <iostream>

void testSemantic(const std::string& source, const std::string& description, bool shouldPass) {
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
    
    // Semantic Analysis
    SemanticAnalyzer analyzer;
    analyzer.analyze(program);
    
    if (analyzer.hadError()) {
        std::cout << "Semantic Errors:" << std::endl;
        for (const auto& error : analyzer.getErrors()) {
            std::cout << "  - " << error << std::endl;
        }
        
        if (shouldPass) {
            std::cout << "❌ FAILED: Expected no errors, but found errors" << std::endl;
        } else {
            std::cout << "✅ PASSED: Expected errors, found errors" << std::endl;
        }
    } else {
        std::cout << "✅ Semantic analysis passed!" << std::endl;
        
        if (!shouldPass) {
            std::cout << "❌ FAILED: Expected errors, but found none" << std::endl;
        }
    }
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CINEBREW Semantic Analyzer Test" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Test 1: Valid program
    testSemantic(
        "TAKE x = 5;\n"
        "TAKE y = 10;\n"
        "TAKE sum = x + y;\n"
        "POUR sum;",
        "Test 1: Valid Program (should pass)",
        true
    );
    
    // Test 2: Undefined variable
    testSemantic(
        "TAKE x = y;",
        "Test 2: Undefined Variable (should fail)",
        false
    );
    
    // Test 3: Valid function call
    testSemantic(
        "SCENE add(a, b) {\n"
        "    SHOT a + b;\n"
        "}\n"
        "TAKE result = add(3, 5);",
        "Test 3: Valid Function Call (should pass)",
        true
    );
    
    // Test 4: Undefined function
    testSemantic(
        "TAKE result = add(3, 5);",
        "Test 4: Undefined Function (should fail)",
        false
    );
    
    // Test 5: Wrong argument count
    testSemantic(
        "SCENE add(a, b) {\n"
        "    SHOT a + b;\n"
        "}\n"
        "TAKE result = add(3);",
        "Test 5: Wrong Argument Count (should fail)",
        false
    );
    
    // Test 6: Redeclaration
    testSemantic(
        "TAKE x = 5;\n"
        "TAKE x = 10;",
        "Test 6: Variable Redeclaration (should fail)",
        false
    );
    
    // Test 7: Using variable before declaration
    testSemantic(
        "TAKE x = y;\n"
        "TAKE y = 5;",
        "Test 7: Use Before Declaration (should fail)",
        false
    );
    
    // Test 8: Valid nested function calls
    testSemantic(
        "SCENE add(a, b) {\n"
        "    SHOT a + b;\n"
        "}\n"
        "SCENE multiply(a, b) {\n"
        "    SHOT a * b;\n"
        "}\n"
        "TAKE result = add(3, multiply(2, 4));",
        "Test 8: Nested Function Calls (should pass)",
        true
    );
    
    // Test 9: Function redeclaration
    testSemantic(
        "SCENE add(a, b) {\n"
        "    SHOT a + b;\n"
        "}\n"
        "SCENE add(x) {\n"
        "    SHOT x;\n"
        "}",
        "Test 9: Function Redeclaration (should fail)",
        false
    );
    
    // Test 10: Complex valid program
    testSemantic(
        "SCENE square(x) {\n"
        "    SHOT x * x;\n"
        "}\n"
        "TAKE num = 5;\n"
        "TAKE squared = square(num);\n"
        "IF squared > 10 {\n"
        "    POUR squared;\n"
        "}",
        "Test 10: Complex Valid Program (should pass)",
        true
    );
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

