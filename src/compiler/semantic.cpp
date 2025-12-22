/**
 * Semantic Analyzer Implementation
 * 
 * Performs semantic checks on the AST.
 */

#include "semantic.h"
#include "../runtime/runtime.h"
#include <iostream>

// ============================================================================
// CONSTRUCTOR
// ============================================================================

SemanticAnalyzer::SemanticAnalyzer() : hadError_(false) {
    // Initialize runtime to check for built-in functions
    runtime_ = std::make_unique<Runtime>();
}

// ============================================================================
// ERROR REPORTING
// ============================================================================

void SemanticAnalyzer::error(const Token& token, const std::string& message) {
    hadError_ = true;
    std::string errorMsg = "Line " + std::to_string(token.line) + ": " + message;
    errors_.push_back(errorMsg);
    std::cerr << "Semantic Error: " << errorMsg << std::endl;
}

// ============================================================================
// SYMBOL TABLE MANAGEMENT
// ============================================================================

void SemanticAnalyzer::declare(const Token& name, SymbolType type, int paramCount) {
    std::string nameStr = name.lexeme;
    
    // Check for redeclaration
    if (symbols_.find(nameStr) != symbols_.end()) {
        Symbol& existing = symbols_[nameStr];
        error(name, "Redeclaration of '" + nameStr + "' (first declared at line " + 
              std::to_string(existing.line) + ")");
        return;
    }
    
    symbols_[nameStr] = Symbol(type, nameStr, name.line, paramCount);
}

Symbol* SemanticAnalyzer::resolve(const Token& name, SymbolType expectedType) {
    std::string nameStr = name.lexeme;
    
    auto it = symbols_.find(nameStr);
    if (it == symbols_.end()) {
        std::string typeStr = (expectedType == SymbolType::VARIABLE ? "variable" : "function");
        error(name, "Undefined " + typeStr + ": '" + nameStr + "'");
        return nullptr;
    }
    
    Symbol& symbol = it->second;
    if (symbol.type != expectedType) {
        error(name, "'" + nameStr + "' is a " + 
              (symbol.type == SymbolType::VARIABLE ? "variable" : "function") + 
              ", not a " + 
              (expectedType == SymbolType::VARIABLE ? "variable" : "function"));
        return nullptr;
    }
    
    return &symbol;
}

// ============================================================================
// MAIN ANALYSIS FUNCTION
// ============================================================================

void SemanticAnalyzer::analyze(std::unique_ptr<Program>& program) {
    symbols_.clear();
    errors_.clear();
    hadError_ = false;
    
    visitProgram(program.get());
}

// ============================================================================
// PROGRAM WALKING
// ============================================================================

void SemanticAnalyzer::visitProgram(Program* program) {
    // First pass: declare all functions
    for (auto& stmt : program->statements) {
        if (FunctionStmt* func = dynamic_cast<FunctionStmt*>(stmt.get())) {
            declare(func->name, SymbolType::FUNCTION, func->parameters.size());
        }
    }
    
    // Second pass: analyze all statements
    for (auto& stmt : program->statements) {
        visitStmt(stmt.get());
    }
}

void SemanticAnalyzer::visitStmt(Stmt* stmt) {
    if (DeclarationStmt* decl = dynamic_cast<DeclarationStmt*>(stmt)) {
        visitDeclaration(decl);
    } else if (AssignmentStmt* assign = dynamic_cast<AssignmentStmt*>(stmt)) {
        visitAssignment(assign);
    } else if (PrintStmt* print = dynamic_cast<PrintStmt*>(stmt)) {
        visitPrint(print);
    } else if (IfStmt* ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        visitIf(ifStmt);
    } else if (LoopStmt* loop = dynamic_cast<LoopStmt*>(stmt)) {
        visitLoop(loop);
    } else if (BreakStmt* brk = dynamic_cast<BreakStmt*>(stmt)) {
        visitBreak(brk);
    } else if (ContinueStmt* cont = dynamic_cast<ContinueStmt*>(stmt)) {
        visitContinue(cont);
    } else if (ReturnStmt* ret = dynamic_cast<ReturnStmt*>(stmt)) {
        visitReturn(ret);
    } else if (FunctionStmt* func = dynamic_cast<FunctionStmt*>(stmt)) {
        visitFunction(func);
    } else if (BlockStmt* block = dynamic_cast<BlockStmt*>(stmt)) {
        visitBlock(block);
    } else if (ExpressionStmt* expr = dynamic_cast<ExpressionStmt*>(stmt)) {
        visitExpr(expr->expression.get());
    }
}

// ============================================================================
// STATEMENT VISITORS
// ============================================================================

void SemanticAnalyzer::visitDeclaration(DeclarationStmt* stmt) {
    // Check initializer expression
    visitExpr(stmt->initializer.get());
    
    // Declare variable
    declare(stmt->name, SymbolType::VARIABLE);
}

void SemanticAnalyzer::visitAssignment(AssignmentStmt* stmt) {
    // Check that variable exists
    resolve(stmt->name, SymbolType::VARIABLE);
    
    // Check value expression
    visitExpr(stmt->value.get());
}

void SemanticAnalyzer::visitPrint(PrintStmt* stmt) {
    visitExpr(stmt->expression.get());
}

void SemanticAnalyzer::visitIf(IfStmt* stmt) {
    visitExpr(stmt->condition.get());
    visitBlock(stmt->thenBranch.get());
    if (stmt->elseBranch) {
        visitBlock(stmt->elseBranch.get());
    }
}

void SemanticAnalyzer::visitLoop(LoopStmt* stmt) {
    visitExpr(stmt->condition.get());
    visitBlock(stmt->body.get());
}

void SemanticAnalyzer::visitBreak(BreakStmt* stmt) {
    // Break is valid (we don't check if it's inside a loop for simplicity)
}

void SemanticAnalyzer::visitContinue(ContinueStmt* stmt) {
    // Continue is valid (we don't check if it's inside a loop for simplicity)
}

void SemanticAnalyzer::visitReturn(ReturnStmt* stmt) {
    if (stmt->value) {
        visitExpr(stmt->value.get());
    }
    // Note: We don't check if we're inside a function for simplicity
}

void SemanticAnalyzer::visitFunction(FunctionStmt* stmt) {
    // Function already declared in first pass
    // Analyze function body
    visitBlock(stmt->body.get());
}

void SemanticAnalyzer::visitBlock(BlockStmt* stmt) {
    for (auto& stmt : stmt->statements) {
        visitStmt(stmt.get());
    }
}

// ============================================================================
// EXPRESSION VISITORS
// ============================================================================

void SemanticAnalyzer::visitExpr(Expr* expr) {
    if (LiteralExpr* lit = dynamic_cast<LiteralExpr*>(expr)) {
        visitLiteral(lit);
    } else if (VariableExpr* var = dynamic_cast<VariableExpr*>(expr)) {
        visitVariable(var);
    } else if (BinaryExpr* bin = dynamic_cast<BinaryExpr*>(expr)) {
        visitBinary(bin);
    } else if (UnaryExpr* un = dynamic_cast<UnaryExpr*>(expr)) {
        visitUnary(un);
    } else if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
        visitCall(call);
    }
}

void SemanticAnalyzer::visitLiteral(LiteralExpr* expr) {
    // Literals are always valid
}

void SemanticAnalyzer::visitVariable(VariableExpr* expr) {
    // Check that variable exists
    resolve(expr->name, SymbolType::VARIABLE);
}

void SemanticAnalyzer::visitBinary(BinaryExpr* expr) {
    visitExpr(expr->left.get());
    visitExpr(expr->right.get());
}

void SemanticAnalyzer::visitUnary(UnaryExpr* expr) {
    visitExpr(expr->right.get());
}

void SemanticAnalyzer::visitCall(CallExpr* expr) {
    std::string funcName = expr->callee.lexeme;
    
    // Check if it's a built-in function
    if (runtime_->isBuiltin(funcName)) {
        int expectedArgs = runtime_->getParamCount(funcName);
        int actualArgs = expr->arguments.size();
        
        if (actualArgs != expectedArgs) {
            error(expr->callee, "Built-in function '" + funcName + "' expects " + 
                  std::to_string(expectedArgs) + " argument(s), but got " + 
                  std::to_string(actualArgs));
        }
    } else {
        // Check that user-defined function exists
        Symbol* symbol = resolve(expr->callee, SymbolType::FUNCTION);
        
        if (symbol) {
            // Check argument count
            int expectedArgs = symbol->paramCount;
            int actualArgs = expr->arguments.size();
            
            if (actualArgs != expectedArgs) {
                error(expr->callee, "Function '" + funcName + "' expects " + 
                      std::to_string(expectedArgs) + " argument(s), but got " + 
                      std::to_string(actualArgs));
            }
        }
    }
    
    // Check all argument expressions
    for (auto& arg : expr->arguments) {
        visitExpr(arg.get());
    }
}

