/**
 * CINEBREW Lexer Implementation
 * 
 * This file implements the complete lexer that tokenizes CineBrew source code.
 */

#include "lexer.h"
#include <iostream>
#include <cctype>

// ============================================================================
// KEYWORD LOOKUP TABLE
// ============================================================================

std::unordered_map<std::string, TokenType> Lexer::keywords_;

void Lexer::initializeKeywords() {
    if (!keywords_.empty()) return;  // Already initialized
    
    keywords_["TAKE"] = TokenType::TAKE;
    keywords_["POUR"] = TokenType::POUR;
    keywords_["SCENE"] = TokenType::SCENE;
    keywords_["SHOT"] = TokenType::SHOT;
    keywords_["IF"] = TokenType::IF;
    keywords_["ELSE"] = TokenType::ELSE;
    keywords_["LOOP"] = TokenType::LOOP;
    keywords_["BREAK"] = TokenType::BREAK;
    keywords_["CONTINUE"] = TokenType::CONTINUE;
    keywords_["true"] = TokenType::TRUE_KW;
    keywords_["false"] = TokenType::FALSE_KW;
}

// ============================================================================
// CONSTRUCTOR
// ============================================================================

Lexer::Lexer(const std::string& source)
    : source_(source), start_(0), current_(0), line_(1), hadError_(false) {
    initializeKeywords();
}

// ============================================================================
// MAIN TOKENIZATION FUNCTION
// ============================================================================

std::vector<Token> Lexer::tokenize() {
    tokens_.clear();
    start_ = 0;
    current_ = 0;
    line_ = 1;
    hadError_ = false;
    errorMessage_ = "";
    
    while (!isAtEnd()) {
        start_ = current_;
        scanToken();
    }
    
    // Add EOF token
    tokens_.push_back(Token(TokenType::END_OF_FILE, "", line_));
    
    return tokens_;
}

// ============================================================================
// CHARACTER CLASSIFICATION
// ============================================================================

bool Lexer::isAtEnd() const {
    return current_ >= (int)source_.length();
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    return source_[current_++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= (int)source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    return true;
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

// ============================================================================
// TOKEN CREATION
// ============================================================================

void Lexer::addToken(TokenType type) {
    std::string lexeme = getCurrentLexeme();
    tokens_.push_back(Token(type, lexeme, line_));
}

void Lexer::addToken(TokenType type, const std::string& literal) {
    std::string lexeme = getCurrentLexeme();
    tokens_.push_back(Token(type, lexeme, literal, line_));
}

void Lexer::error(const std::string& message) {
    hadError_ = true;
    errorMessage_ = "Line " + std::to_string(line_) + ": " + message;
    std::cerr << "Lexer Error: " << errorMessage_ << std::endl;
}

std::string Lexer::getCurrentLexeme() const {
    return source_.substr(start_, current_ - start_);
}

// ============================================================================
// TOKEN SCANNING
// ============================================================================

void Lexer::scanToken() {
    char c = advance();
    
    switch (c) {
        // ====================================================================
        // WHITESPACE
        // ====================================================================
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;
            
        case '\n':
            line_++;
            break;
            
        // ====================================================================
        // COMMENTS
        // ====================================================================
        case '#':
            // Comment: skip until end of line
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
            break;
            
        // ====================================================================
        // SINGLE-CHARACTER TOKENS
        // ====================================================================
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
            
        case '(':
            addToken(TokenType::LPAREN);
            break;
            
        case ')':
            addToken(TokenType::RPAREN);
            break;
            
        case '{':
            addToken(TokenType::LBRACE);
            break;
            
        case '}':
            addToken(TokenType::RBRACE);
            break;
            
        case ',':
            addToken(TokenType::COMMA);
            break;
            
        // ====================================================================
        // OPERATORS (may be multi-character)
        // ====================================================================
        case '+':
            addToken(TokenType::PLUS);
            break;
            
        case '-':
            // Could be subtraction or negative number
            if (isDigit(peek())) {
                // Negative number: -42
                scanNumber();
            } else {
                // Subtraction operator
                addToken(TokenType::MINUS);
            }
            break;
            
        case '*':
            addToken(TokenType::STAR);
            break;
            
        case '/':
            addToken(TokenType::SLASH);
            break;
            
        case '=':
            if (match('=')) {
                addToken(TokenType::EQUAL_EQUAL);  // ==
            } else {
                addToken(TokenType::EQUAL);       // =
            }
            break;
            
        case '!':
            if (match('=')) {
                addToken(TokenType::BANG_EQUAL);  // !=
            } else {
                error("Unexpected character '!' (did you mean '!='?)");
                addToken(TokenType::ERROR);
            }
            break;
            
        case '>':
            if (match('=')) {
                addToken(TokenType::GREATER_EQUAL);  // >=
            } else {
                addToken(TokenType::GREATER);       // >
            }
            break;
            
        case '<':
            if (match('=')) {
                addToken(TokenType::LESS_EQUAL);     // <=
            } else {
                addToken(TokenType::LESS);           // <
            }
            break;
            
        // ====================================================================
        // STRING LITERAL
        // ====================================================================
        case '"':
            scanString();
            break;
            
        // ====================================================================
        // NUMBERS AND IDENTIFIERS
        // ====================================================================
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c)) {
                scanIdentifier();
            } else {
                error("Unexpected character: '" + std::string(1, c) + "'");
                addToken(TokenType::ERROR);
            }
            break;
    }
}

// ============================================================================
// SCAN STRING LITERAL
// ============================================================================

void Lexer::scanString() {
    // We've already consumed the opening "
    // Now scan until closing "
    
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line_++;
        }
        advance();
    }
    
    // Check if we found the closing quote
    if (isAtEnd()) {
        error("Unterminated string literal");
        return;
    }
    
    // Consume the closing "
    advance();
    
    // Extract string value (without quotes)
    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    addToken(TokenType::STRING, value);
}

// ============================================================================
// SCAN NUMBER LITERAL
// ============================================================================

void Lexer::scanNumber() {
    // We've already consumed the first digit (or minus sign)
    // Continue consuming digits
    
    while (isDigit(peek())) {
        advance();
    }
    
    // Check for decimal point (not supported yet, but prepare for future)
    if (peek() == '.' && isDigit(peekNext())) {
        // Decimal number (for future)
        advance();  // Consume '.'
        while (isDigit(peek())) {
            advance();
        }
        error("Floating point numbers not yet supported");
    }
    
    // Extract number value
    std::string value = getCurrentLexeme();
    addToken(TokenType::NUMBER, value);
}

// ============================================================================
// SCAN IDENTIFIER OR KEYWORD
// ============================================================================

void Lexer::scanIdentifier() {
    // We've already consumed the first character (letter or underscore)
    // Continue consuming letters, digits, and underscores
    
    while (isAlphaNumeric(peek())) {
        advance();
    }
    
    // Check if it's a keyword
    std::string text = getCurrentLexeme();
    
    // Check for illegal identifier (starts with digit)
    if (isDigit(text[0])) {
        error("Identifier cannot start with a digit: " + text);
        addToken(TokenType::ERROR);
        return;
    }
    
    // Look up in keyword table
    auto it = keywords_.find(text);
    if (it != keywords_.end()) {
        // It's a keyword
        addToken(it->second);
    } else {
        // It's an identifier
        addToken(TokenType::IDENTIFIER);
    }
}

