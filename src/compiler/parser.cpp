/**
 * CINEBREW Parser Implementation
 * 
 * Recursive descent parser that builds AST from token stream.
 */

#include "parser.h"
#include <iostream>

// ============================================================================
// CONSTRUCTOR
// ============================================================================

Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens), current_(0), hadError_(false) {
}

// ============================================================================
// TOKEN ACCESS
// ============================================================================

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() const {
    return tokens_[current_];
}

Token Parser::previous() const {
    return tokens_[current_ - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    error(peek(), message);
    return Token(type, "", peek().line);
}

// ============================================================================
// ERROR HANDLING
// ============================================================================

void Parser::error(const Token& token, const std::string& message) {
    if (hadError_) return;  // Don't report multiple errors
    
    hadError_ = true;
    errorMessage_ = "Line " + std::to_string(token.line) + ": " + message;
    std::cerr << "Parse Error: " << errorMessage_ << std::endl;
}

void Parser::synchronize() {
    // Skip tokens until we find a statement boundary
    advance();
    
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        
        switch (peek().type) {
            case TokenType::TAKE:
            case TokenType::POUR:
            case TokenType::SCENE:
            case TokenType::IF:
            case TokenType::LOOP:
            case TokenType::BREAK:
            case TokenType::CONTINUE:
            case TokenType::SHOT:
                return;
            default:
                break;
        }
        
        advance();
    }
}

// ============================================================================
// TOP-LEVEL PARSING
// ============================================================================

std::unique_ptr<Program> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::declaration() {
    if (match(TokenType::SCENE)) {
        return functionStmt();
    }
    if (match(TokenType::TAKE)) {
        return declarationStmt();
    }
    return statement();
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match(TokenType::POUR)) return printStmt();
    if (match(TokenType::IF)) return ifStmt();
    if (match(TokenType::LOOP)) return loopStmt();
    if (match(TokenType::BREAK)) return breakStmt();
    if (match(TokenType::CONTINUE)) return continueStmt();
    if (match(TokenType::SHOT)) return returnStmt();
    if (match(TokenType::LBRACE)) return block();
    
    // Try assignment or expression statement
    return assignmentStmt();
}

// ============================================================================
// STATEMENT PARSING
// ============================================================================

std::unique_ptr<Stmt> Parser::declarationStmt() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::EQUAL, "Expected '=' after variable name");
    std::unique_ptr<Expr> initializer = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    
    return std::make_unique<DeclarationStmt>(
        previous(), name, std::move(initializer)
    );
}

std::unique_ptr<Stmt> Parser::assignmentStmt() {
    // Try to parse as assignment: identifier = expression
    Token name = peek();
    
    if (check(TokenType::IDENTIFIER) && peekNext().type == TokenType::EQUAL) {
        advance();  // Consume identifier
        advance();  // Consume =
        std::unique_ptr<Expr> value = expression();
        consume(TokenType::SEMICOLON, "Expected ';' after assignment");
        
        return std::make_unique<AssignmentStmt>(name, std::move(value));
    }
    
    // Otherwise, it's an expression statement
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

Token Parser::peekNext() const {
    if (current_ + 1 >= (int)tokens_.size()) {
        return Token(TokenType::END_OF_FILE, "", tokens_.back().line);
    }
    return tokens_[current_ + 1];
}

std::unique_ptr<Stmt> Parser::printStmt() {
    Token keyword = previous();
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after POUR statement");
    
    return std::make_unique<PrintStmt>(keyword, std::move(expr));
}

std::unique_ptr<Stmt> Parser::ifStmt() {
    Token keyword = previous();
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::LBRACE, "Expected '{' after IF condition");
    std::unique_ptr<BlockStmt> thenBranch = block();
    
    std::unique_ptr<BlockStmt> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        consume(TokenType::LBRACE, "Expected '{' after ELSE");
        elseBranch = block();
    }
    
    return std::make_unique<IfStmt>(
        keyword, std::move(condition), std::move(thenBranch), std::move(elseBranch)
    );
}

std::unique_ptr<Stmt> Parser::loopStmt() {
    Token keyword = previous();
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::LBRACE, "Expected '{' after LOOP condition");
    std::unique_ptr<BlockStmt> body = block();
    
    return std::make_unique<LoopStmt>(keyword, std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::breakStmt() {
    Token keyword = previous();
    consume(TokenType::SEMICOLON, "Expected ';' after BREAK");
    return std::make_unique<BreakStmt>(keyword);
}

std::unique_ptr<Stmt> Parser::continueStmt() {
    Token keyword = previous();
    consume(TokenType::SEMICOLON, "Expected ';' after CONTINUE");
    return std::make_unique<ContinueStmt>(keyword);
}

std::unique_ptr<Stmt> Parser::returnStmt() {
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after SHOT statement");
    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

std::unique_ptr<Stmt> Parser::functionStmt() {
    Token keyword = previous();
    Token name = consume(TokenType::IDENTIFIER, "Expected function name");
    consume(TokenType::LPAREN, "Expected '(' after function name");
    
    std::vector<Token> parameters;
    if (!check(TokenType::RPAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Cannot have more than 255 parameters");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name"));
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RPAREN, "Expected ')' after parameters");
    consume(TokenType::LBRACE, "Expected '{' before function body");
    std::unique_ptr<BlockStmt> body = block();
    
    return std::make_unique<FunctionStmt>(
        keyword, name, std::move(parameters), std::move(body)
    );
}

std::unique_ptr<BlockStmt> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    
    consume(TokenType::RBRACE, "Expected '}' after block");
    return std::make_unique<BlockStmt>(std::move(statements));
}

// ============================================================================
// EXPRESSION PARSING (with precedence)
// ============================================================================

std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment() {
    // For now, we don't support assignment as expression
    // (like: x = y = 5)
    return comparison();
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = addition();
    
    while (match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) ||
           match(TokenType::LESS) || match(TokenType::LESS_EQUAL) ||
           match(TokenType::EQUAL_EQUAL) || match(TokenType::BANG_EQUAL)) {
        Token op = previous();
        std::unique_ptr<Expr> right = addition();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::addition() {
    std::unique_ptr<Expr> expr = multiplication();
    
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previous();
        std::unique_ptr<Expr> right = multiplication();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::multiplication() {
    std::unique_ptr<Expr> expr = unary();
    
    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match(TokenType::MINUS)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }
    
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    // Literals
    if (match(TokenType::NUMBER)) {
        return std::make_unique<LiteralExpr>(previous(), previous().literal);
    }
    
    if (match(TokenType::TRUE_KW)) {
        return std::make_unique<LiteralExpr>(previous(), "true");
    }
    
    if (match(TokenType::FALSE_KW)) {
        return std::make_unique<LiteralExpr>(previous(), "false");
    }
    
    if (match(TokenType::STRING)) {
        return std::make_unique<LiteralExpr>(previous(), previous().literal);
    }
    
    // Variables
    if (match(TokenType::IDENTIFIER)) {
        Token name = previous();
        
        // Check if it's a function call
        if (match(TokenType::LPAREN)) {
            return finishCall(name);
        }
        
        return std::make_unique<VariableExpr>(name);
    }
    
    // Grouping
    if (match(TokenType::LPAREN)) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    error(peek(), "Expected expression");
    return std::make_unique<LiteralExpr>(Token(TokenType::ERROR, "", peek().line), "0");
}

std::unique_ptr<Expr> Parser::finishCall(const Token& callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    
    if (!check(TokenType::RPAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Cannot have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    
    Token paren = consume(TokenType::RPAREN, "Expected ')' after arguments");
    return std::make_unique<CallExpr>(callee, std::move(arguments));
}

