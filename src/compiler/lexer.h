/**

/**
 * CINEBREW Lexer
 * 
 * Tokenizes source code into token stream.
 */
#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>
#include <string>
#include <unordered_map>

/**
 * Lexer Class
 * 
 * Converts CineBrew source code into a stream of tokens.
 */
class Lexer {
public:
    Lexer(const std::string& source);
    
    // Main function: tokenize entire source code
    std::vector<Token> tokenize();
    
    // Check if there were any errors
    bool hadError() const { return hadError_; }
    
    // Get error message (if any)
    std::string getError() const { return errorMessage_; }

private:
    // ========================================================================
    // STATE
    // ========================================================================
    std::string source_;           // Source code to tokenize
    std::vector<Token> tokens_;    // Generated tokens
    int start_;                    // Start of current token
    int current_;                   // Current position in source
    int line_;                      // Current line number
    bool hadError_;                 // Error flag
    std::string errorMessage_;     // Error message
    
    // Keyword lookup table
    static std::unordered_map<std::string, TokenType> keywords_;
    static void initializeKeywords();
    
    // ========================================================================
    // CHARACTER CLASSIFICATION
    // ========================================================================
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    
    // ========================================================================
    // TOKEN CREATION
    // ========================================================================
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& literal);
    void error(const std::string& message);
    
    // ========================================================================
    // TOKEN SCANNING
    // ========================================================================
    void scanToken();
    void scanString();      // Scan string literal: "hello"
    void scanNumber();      // Scan number: 42, -10
    void scanIdentifier();  // Scan identifier/keyword: x, TAKE
    void scanOperator();    // Scan operators: +, ==, !=, etc.
    
    // ========================================================================
    // HELPER FUNCTIONS
    // ========================================================================
    std::string getCurrentLexeme() const;
};

#endif // LEXER_H

