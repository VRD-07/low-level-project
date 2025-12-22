/**
 * CINEBREW Parser
 * 
 * ============================================================================
 * WHAT IS PARSING?
 * ============================================================================
 * 
 * Parsing is the process of analyzing a sequence of tokens to determine
 * their grammatical structure according to the language's grammar rules.
 * 
 * Example:
 *   Tokens: [TAKE, IDENTIFIER(x), EQUAL, NUMBER(5), SEMICOLON]
 *   AST:    DeclarationStmt(TAKE, x, LiteralExpr(5))
 * 
 * HOW IT WORKS
 * 
 * We use "recursive descent" parsing:
 * - Each grammar rule becomes a function
 * - Functions call each other recursively
 * - Builds AST nodes as it parses
 * 
 * ============================================================================
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>

/**
 * Parser Class
 * 
 * Converts token stream into Abstract Syntax Tree (AST).
 */
class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    
    // Main function: parse entire program
    std::unique_ptr<Program> parse();
    
    // Check if there were any errors
    bool hadError() const { return hadError_; }
    
    // Get error message (if any)
    std::string getError() const { return errorMessage_; }

private:
    // ========================================================================
    // STATE
    // ========================================================================
    std::vector<Token> tokens_;
    int current_;
    bool hadError_;
    std::string errorMessage_;
    
    // ========================================================================
    // TOKEN ACCESS
    // ========================================================================
    bool isAtEnd() const;
    Token peek() const;
    Token peekNext() const;  // Look ahead one token
    Token previous() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    
    // ========================================================================
    // ERROR HANDLING
    // ========================================================================
    void error(const Token& token, const std::string& message);
    void synchronize();  // Error recovery
    
    // ========================================================================
    // PARSING FUNCTIONS (Recursive Descent)
    // ========================================================================
    
    // Top-level
    std::unique_ptr<Program> program();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> statement();
    
    // Statements
    std::unique_ptr<Stmt> declarationStmt();
    std::unique_ptr<Stmt> assignmentStmt();
    std::unique_ptr<Stmt> printStmt();
    std::unique_ptr<Stmt> ifStmt();
    std::unique_ptr<Stmt> loopStmt();
    std::unique_ptr<Stmt> breakStmt();
    std::unique_ptr<Stmt> continueStmt();
    std::unique_ptr<Stmt> returnStmt();
    std::unique_ptr<Stmt> functionStmt();
    std::unique_ptr<BlockStmt> block();
    
    // Expressions
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Expr> finishCall(const Token& callee);
};

#endif // PARSER_H

