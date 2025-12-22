/**
 * CINEBREW Abstract Syntax Tree (AST)
 * 
 * ============================================================================
 * WHAT IS AN AST?
 * ============================================================================
 * 
 * An Abstract Syntax Tree (AST) is a tree representation of source code
 * that captures the structure and meaning of the program.
 * 
 * Example:
 *   Source: "TAKE x = 3 + 5;"
 *   AST:
 *        Declaration
 *        /    |    \
 *     TAKE    x    BinaryExpr
 *                      /    |    \
 *                     3     +     5
 * 
 * WHY DO WE NEED IT?
 * 
 * 1. Represents program structure clearly
 * 2. Separates syntax from semantics
 * 3. Makes code generation easier
 * 4. Enables syntax validation
 * 
 * ============================================================================
 */

#ifndef AST_H
#define AST_H

#include "token.h"
#include <vector>
#include <string>
#include <memory>

// Forward declarations
class Expr;
class Stmt;
class BlockStmt;  // Forward declaration for BlockStmt

// ============================================================================
// EXPRESSION NODES
// ============================================================================

/**
 * Base class for all expressions
 */
class Expr {
public:
    virtual ~Expr() = default;
    virtual std::string toString() const = 0;
};

/**
 * Literal expression: 42, true, false, "hello"
 */
class LiteralExpr : public Expr {
public:
    Token token;
    std::string value;
    
    LiteralExpr(const Token& tok, const std::string& val)
        : token(tok), value(val) {}
    
    std::string toString() const override;
};

/**
 * Variable expression: x, myVar
 */
class VariableExpr : public Expr {
public:
    Token name;
    
    VariableExpr(const Token& tok) : name(tok) {}
    
    std::string toString() const override;
};

/**
 * Binary expression: a + b, x > 0, etc.
 */
class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    
    BinaryExpr(std::unique_ptr<Expr> l, const Token& o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
    
    std::string toString() const override;
};

/**
 * Unary expression: -x, !x
 */
class UnaryExpr : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;
    
    UnaryExpr(const Token& o, std::unique_ptr<Expr> r)
        : op(o), right(std::move(r)) {}
    
    std::string toString() const override;
};

/**
 * Function call expression: add(3, 5)
 */
class CallExpr : public Expr {
public:
    Token callee;  // Function name
    std::vector<std::unique_ptr<Expr>> arguments;
    
    CallExpr(const Token& tok, std::vector<std::unique_ptr<Expr>> args)
        : callee(tok), arguments(std::move(args)) {}
    
    std::string toString() const override;
};

// ============================================================================
// STATEMENT NODES
// ============================================================================

/**
 * Base class for all statements
 */
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual std::string toString() const = 0;
};

/**
 * Variable declaration: TAKE x = 5;
 */
class DeclarationStmt : public Stmt {
public:
    Token keyword;  // TAKE
    Token name;
    std::unique_ptr<Expr> initializer;
    
    DeclarationStmt(const Token& kw, const Token& n, std::unique_ptr<Expr> init)
        : keyword(kw), name(n), initializer(std::move(init)) {}
    
    std::string toString() const override;
};

/**
 * Assignment statement: x = 5;
 */
class AssignmentStmt : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> value;
    
    AssignmentStmt(const Token& n, std::unique_ptr<Expr> val)
        : name(n), value(std::move(val)) {}
    
    std::string toString() const override;
};

/**
 * Expression statement: x + y; (result discarded)
 */
class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;
    
    ExpressionStmt(std::unique_ptr<Expr> expr)
        : expression(std::move(expr)) {}
    
    std::string toString() const override;
};

/**
 * Print statement: POUR x;
 */
class PrintStmt : public Stmt {
public:
    Token keyword;  // POUR
    std::unique_ptr<Expr> expression;
    
    PrintStmt(const Token& kw, std::unique_ptr<Expr> expr)
        : keyword(kw), expression(std::move(expr)) {}
    
    std::string toString() const override;
};

/**
 * If statement: IF condition { ... } ELSE { ... }
 */
class IfStmt : public Stmt {
public:
    Token keyword;  // IF
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> thenBranch;
    std::unique_ptr<BlockStmt> elseBranch;  // Can be null
    
    IfStmt(const Token& kw, std::unique_ptr<Expr> cond,
           std::unique_ptr<BlockStmt> thenBr,
           std::unique_ptr<BlockStmt> elseBr)
        : keyword(kw), condition(std::move(cond)),
          thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}
    
    std::string toString() const override;
};

/**
 * Loop statement: LOOP condition { ... }
 */
class LoopStmt : public Stmt {
public:
    Token keyword;  // LOOP
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> body;
    
    LoopStmt(const Token& kw, std::unique_ptr<Expr> cond,
             std::unique_ptr<BlockStmt> b)
        : keyword(kw), condition(std::move(cond)), body(std::move(b)) {}
    
    std::string toString() const override;
};

/**
 * Break statement: BREAK;
 */
class BreakStmt : public Stmt {
public:
    Token keyword;  // BREAK
    
    BreakStmt(const Token& kw) : keyword(kw) {}
    
    std::string toString() const override;
};

/**
 * Continue statement: CONTINUE;
 */
class ContinueStmt : public Stmt {
public:
    Token keyword;  // CONTINUE
    
    ContinueStmt(const Token& kw) : keyword(kw) {}
    
    std::string toString() const override;
};

/**
 * Return statement: SHOT value;
 */
class ReturnStmt : public Stmt {
public:
    Token keyword;  // SHOT
    std::unique_ptr<Expr> value;  // Can be null
    
    ReturnStmt(const Token& kw, std::unique_ptr<Expr> val)
        : keyword(kw), value(std::move(val)) {}
    
    std::string toString() const override;
};

/**
 * Block statement: { statement1; statement2; ... }
 */
class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
    
    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts)
        : statements(std::move(stmts)) {}
    
    std::string toString() const override;
};

/**
 * Function definition: SCENE name(params) { ... }
 */
class FunctionStmt : public Stmt {
public:
    Token keyword;  // SCENE
    Token name;
    std::vector<Token> parameters;
    std::unique_ptr<BlockStmt> body;
    
    FunctionStmt(const Token& kw, const Token& n,
                 std::vector<Token> params, std::unique_ptr<BlockStmt> b)
        : keyword(kw), name(n), parameters(std::move(params)), body(std::move(b)) {}
    
    std::string toString() const override;
};

// ============================================================================
// PROGRAM NODE
// ============================================================================

/**
 * Program: Top-level node containing all statements
 */
class Program {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
    
    Program(std::vector<std::unique_ptr<Stmt>> stmts)
        : statements(std::move(stmts)) {}
    
    std::string toString() const;
};

#endif // AST_H

