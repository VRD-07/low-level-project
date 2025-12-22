/**
 * Token Implementation
 * Helper functions for tokens
 */

#include "token.h"
#include <unordered_map>

std::string tokenTypeToString(TokenType type) {
    static std::unordered_map<TokenType, std::string> typeNames = {
        {TokenType::TAKE, "TAKE"},
        {TokenType::POUR, "POUR"},
        {TokenType::SCENE, "SCENE"},
        {TokenType::SHOT, "SHOT"},
        {TokenType::IF, "IF"},
        {TokenType::ELSE, "ELSE"},
        {TokenType::LOOP, "LOOP"},
        {TokenType::BREAK, "BREAK"},
        {TokenType::CONTINUE, "CONTINUE"},
        {TokenType::TRUE_KW, "TRUE"},
        {TokenType::FALSE_KW, "FALSE"},
        {TokenType::IDENTIFIER, "IDENTIFIER"},
        {TokenType::NUMBER, "NUMBER"},
        {TokenType::STRING, "STRING"},
        {TokenType::EQUAL, "EQUAL"},
        {TokenType::PLUS, "PLUS"},
        {TokenType::MINUS, "MINUS"},
        {TokenType::STAR, "STAR"},
        {TokenType::SLASH, "SLASH"},
        {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
        {TokenType::BANG_EQUAL, "BANG_EQUAL"},
        {TokenType::GREATER, "GREATER"},
        {TokenType::LESS, "LESS"},
        {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
        {TokenType::LESS_EQUAL, "LESS_EQUAL"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::LBRACE, "LBRACE"},
        {TokenType::RBRACE, "RBRACE"},
        {TokenType::LPAREN, "LPAREN"},
        {TokenType::RPAREN, "RPAREN"},
        {TokenType::COMMA, "COMMA"},
        {TokenType::END_OF_FILE, "END_OF_FILE"},
        {TokenType::ERROR, "ERROR"}
    };
    
    auto it = typeNames.find(type);
    if (it != typeNames.end()) {
        return it->second;
    }
    return "UNKNOWN";
}

