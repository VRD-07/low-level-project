/**
 * Lexer Test Program
 * 
 * Tests the CineBrew lexer with various source code examples.
 */

#include "../src/compiler/lexer.h"
#include "../src/compiler/token.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printTokens(const std::vector<Token>& tokens) {
    std::cout << "Tokens (" << tokens.size() << " total):" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    for (size_t i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];
        std::cout << "[" << i << "] " << tokenTypeToString(token.type);
        std::cout << " \"" << token.lexeme << "\"";
        
        if (!token.literal.empty()) {
            std::cout << " (literal: " << token.literal << ")";
        }
        
        std::cout << " [line " << token.line << "]" << std::endl;
    }
    
    std::cout << "----------------------------------------" << std::endl;
}

void testLexer(const std::string& source, const std::string& description) {
    std::cout << "\n=== " << description << " ===" << std::endl;
    std::cout << "Source code:" << std::endl;
    std::cout << source << std::endl;
    std::cout << std::endl;
    
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    if (lexer.hadError()) {
        std::cout << "ERROR: " << lexer.getError() << std::endl;
    } else {
        printTokens(tokens);
    }
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CINEBREW Lexer Test" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Test 1: Simple variable declaration
    testLexer(
        "TAKE x = 5;",
        "Test 1: Simple Variable Declaration"
    );
    
    // Test 2: Arithmetic expression
    testLexer(
        "TAKE result = a + b;",
        "Test 2: Arithmetic Expression"
    );
    
    // Test 3: Conditional
    testLexer(
        "IF x > 0 {\n    POUR x;\n}",
        "Test 3: Conditional Statement"
    );
    
    // Test 4: Loop
    testLexer(
        "LOOP i < 5 {\n    i = i + 1;\n}",
        "Test 4: Loop Statement"
    );
    
    // Test 5: Function definition
    testLexer(
        "SCENE add(a, b) {\n    SHOT a + b;\n}",
        "Test 5: Function Definition"
    );
    
    // Test 6: Comparison operators
    testLexer(
        "TAKE eq = a == b;\nTAKE ne = a != b;\nTAKE gt = a > b;\nTAKE ge = a >= b;",
        "Test 6: Comparison Operators"
    );
    
    // Test 7: Comments
    testLexer(
        "# This is a comment\nTAKE x = 5; # Another comment",
        "Test 7: Comments"
    );
    
    // Test 8: Negative numbers
    testLexer(
        "TAKE x = -10;\nTAKE y = 20;\nTAKE result = x + y;",
        "Test 8: Negative Numbers"
    );
    
    // Test 9: String literals
    testLexer(
        "POUR \"Hello, World!\";",
        "Test 9: String Literals"
    );
    
    // Test 10: Complex program
    testLexer(
        "TAKE x = 10;\n"
        "TAKE y = 20;\n"
        "IF x > 5 {\n"
        "    TAKE sum = x + y;\n"
        "    POUR sum;\n"
        "}",
        "Test 10: Complex Program"
    );
    
    // Test 11: Error case - illegal identifier
    testLexer(
        "TAKE 123abc = 5;",
        "Test 11: Illegal Identifier (should error)"
    );
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

