/**
 * CINEBREW Semantic Analyzer
 * 
 * ============================================================================
 * WHAT IS SEMANTIC ANALYSIS?
 * ============================================================================
 * 
 * Semantic analysis checks that the program is meaningful, not just
 * syntactically correct.
 * 
 * Syntax vs Semantics:
 *   - Syntax: "Is it grammatically correct?" (Parser checks this)
 *   - Semantics: "Does it make sense?" (This analyzer checks this)
 * 
 * Examples of Semantic Errors:
 *   - Using undefined variable: x = y; (y not defined)
 *   - Calling undefined function: result = add(3, 5); (add not defined)
 *   - Wrong argument count: add(3); (add expects 2 args)
 * 
 * ============================================================================
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "token.h"
#include "../runtime/runtime.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

/**
 * Symbol Information
 * 
 * Tracks information about variables and functions.
 */
enum class SymbolType {
    VARIABLE,
    FUNCTION
};

struct Symbol {
    SymbolType type;
    std::string name;
    int line;  // Where it was declared
    int paramCount;  // For functions: number of parameters
    
    // Default constructor (required for unordered_map::operator[])
    Symbol() : type(SymbolType::VARIABLE), name(""), line(0), paramCount(0) {}
    
    Symbol(SymbolType t, const std::string& n, int l, int params = 0)
        : type(t), name(n), line(l), paramCount(params) {}
};

/**
 * Semantic Analyzer
 * 
 * Walks the AST and performs semantic checks.
 */
class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    
    // Main function: analyze program
    void analyze(std::unique_ptr<Program>& program);
    
    // Check if there were any errors
    bool hadError() const { return hadError_; }
    
    // Get error messages
    std::vector<std::string> getErrors() const { return errors_; }

private:
    // ========================================================================
    // STATE
    // ========================================================================
    std::unordered_map<std::string, Symbol> symbols_;  // Global symbol table
    std::vector<std::string> errors_;
    bool hadError_;
    std::unique_ptr<Runtime> runtime_;  // Runtime library for built-in functions
    
    // ========================================================================
    // ERROR REPORTING
    // ========================================================================
    void error(const Token& token, const std::string& message);
    
    // ========================================================================
    // SYMBOL TABLE MANAGEMENT
    // ========================================================================
    void declare(const Token& name, SymbolType type, int paramCount = 0);
    Symbol* resolve(const Token& name, SymbolType expectedType);
    
    // ========================================================================
    // AST WALKING
    // ========================================================================
    void visitProgram(Program* program);
    void visitStmt(Stmt* stmt);
    void visitExpr(Expr* expr);
    
    // Statements
    void visitDeclaration(DeclarationStmt* stmt);
    void visitAssignment(AssignmentStmt* stmt);
    void visitPrint(PrintStmt* stmt);
    void visitIf(IfStmt* stmt);
    void visitLoop(LoopStmt* stmt);
    void visitBreak(BreakStmt* stmt);
    void visitContinue(ContinueStmt* stmt);
    void visitReturn(ReturnStmt* stmt);
    void visitFunction(FunctionStmt* stmt);
    void visitBlock(BlockStmt* stmt);
    
    // Expressions
    void visitLiteral(LiteralExpr* expr);
    void visitVariable(VariableExpr* expr);
    void visitBinary(BinaryExpr* expr);
    void visitUnary(UnaryExpr* expr);
    void visitCall(CallExpr* expr);
};

#endif // SEMANTIC_H

