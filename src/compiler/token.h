/**
 * CINEBREW Token Definitions
 * 
 * Tokens are the smallest meaningful units of source code.
 * The lexer breaks source code into tokens for the parser.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/**
 * Token Types
 * 
 * These represent all possible tokens in CineBrew source code.
 */
enum class TokenType {
    // ========================================================================
    // KEYWORDS (Cinema + Coffee Theme)
    // ========================================================================
    TAKE,       // Variable declaration: TAKE x = 5;
    POUR,       // Print statement: POUR x;
    SCENE,      // Function definition: SCENE add(a, b) { ... }
    SHOT,       // Return statement: SHOT result;
    IF,         // Conditional: IF x > 0 { ... }
    ELSE,       // Else clause: ELSE { ... }
    LOOP,       // While loop: LOOP i < 10 { ... }
    BREAK,      // Exit loop: BREAK;
    CONTINUE,   // Skip iteration: CONTINUE;
    TRUE_KW,    // Boolean true: true
    FALSE_KW,   // Boolean false: false
    
    // ========================================================================
    // LITERALS
    // ========================================================================
    IDENTIFIER, // Variable/function name: x, myVar, add
    NUMBER,     // Integer literal: 42, -10, 0
    STRING,     // String literal: "hello" (for future use)
    
    // ========================================================================
    // OPERATORS
    // ========================================================================
    EQUAL,      // Assignment: =
    PLUS,       // Addition: +
    MINUS,      // Subtraction: -
    STAR,       // Multiplication: *
    SLASH,      // Division: /
    
    // ========================================================================
    // COMPARISON OPERATORS
    // ========================================================================
    EQUAL_EQUAL,    // Equal: ==
    BANG_EQUAL,     // Not equal: !=
    GREATER,        // Greater than: >
    LESS,           // Less than: <
    GREATER_EQUAL,  // Greater or equal: >=
    LESS_EQUAL,     // Less or equal: <=
    
    // ========================================================================
    // SYMBOLS
    // ========================================================================
    SEMICOLON,  // Statement terminator: ;
    LBRACE,     // Left brace: {
    RBRACE,     // Right brace: }
    LPAREN,     // Left parenthesis: (
    RPAREN,     // Right parenthesis: )
    COMMA,      // Comma: ,
    
    // ========================================================================
    // SPECIAL
    // ========================================================================
    END_OF_FILE,    // End of source code
    ERROR           // Error token (for invalid input)
};

/**
 * Token Structure
 * 
 * Represents a single token with:
 * - type: What kind of token (keyword, identifier, etc.)
 * - lexeme: The actual text from source code
 * - literal: The value (for numbers, strings)
 * - line: Line number where token appears
 */
struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;  // For numbers: the numeric value as string
    int line;
    
    Token(TokenType t, std::string lex, std::string lit, int l)
        : type(t), lexeme(lex), literal(lit), line(l) {}
    
    Token(TokenType t, std::string lex, int l)
        : type(t), lexeme(lex), literal(""), line(l) {}
};

/**
 * Helper function to convert TokenType to string (for debugging)
 */
std::string tokenTypeToString(TokenType type);

#endif // TOKEN_H

