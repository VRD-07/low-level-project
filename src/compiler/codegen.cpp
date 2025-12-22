/**
 * Code Generator Implementation
 * 
 * Translates AST into VM bytecode instructions.
 */

#include "codegen.h"
#include <sstream>
#include <iostream>

// ============================================================================
// CONSTRUCTOR
// ============================================================================

CodeGenerator::CodeGenerator() : hadError_(false) {
}

// ============================================================================
// ERROR HANDLING
// ============================================================================

void CodeGenerator::error(const std::string& message) {
    hadError_ = true;
    errorMessage_ = message;
    std::cerr << "CodeGen Error: " << message << std::endl;
}

// ============================================================================
// BYTECODE GENERATION
// ============================================================================

void CodeGenerator::emit(const std::string& instruction) {
    bytecode_.push_back(instruction);
}

std::string CodeGenerator::newLabel(const std::string& prefix) {
    int count = labelCounter_[prefix]++;
    return prefix + "_" + std::to_string(count);
}

// ============================================================================
// MAIN GENERATION FUNCTION
// ============================================================================

std::vector<std::string> CodeGenerator::generate(std::unique_ptr<Program>& program) {
    bytecode_.clear();
    labelCounter_.clear();
    hadError_ = false;
    errorMessage_ = "";
    
    visitProgram(program.get());
    
    return bytecode_;
}

// ============================================================================
// PROGRAM WALKING
// ============================================================================

void CodeGenerator::visitProgram(Program* program) {
    // Generate code for all statements
    for (auto& stmt : program->statements) {
        visitStmt(stmt.get());
    }
    
    // Add HALT label at end
    emit("HALT:");
}

void CodeGenerator::visitStmt(Stmt* stmt) {
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
        // Expression result is on stack, but we discard it
        // (In a more sophisticated system, we'd pop it)
    }
}

// ============================================================================
// STATEMENT CODE GENERATION
// ============================================================================

void CodeGenerator::visitDeclaration(DeclarationStmt* stmt) {
    // Generate code for initializer expression
    visitExpr(stmt->initializer.get());
    
    // Store in variable
    emit("STORE " + stmt->name.lexeme);
}

void CodeGenerator::visitAssignment(AssignmentStmt* stmt) {
    // Generate code for value expression
    visitExpr(stmt->value.get());
    
    // Store in variable
    emit("STORE " + stmt->name.lexeme);
}

void CodeGenerator::visitPrint(PrintStmt* stmt) {
    // Generate code for expression
    visitExpr(stmt->expression.get());
    
    // Print top of stack
    emit("PRINT");
}

void CodeGenerator::visitIf(IfStmt* stmt) {
    // Generate code for condition
    visitExpr(stmt->condition.get());
    
    // Create labels
    std::string elseLabel = newLabel("else");
    std::string endLabel = newLabel("end_if");
    
    // Jump to else if condition is false (0)
    emit("JZ " + elseLabel);
    
    // Generate then branch
    visitBlock(stmt->thenBranch.get());
    
    // Jump to end (skip else)
    emit("JMP " + endLabel);
    
    // Else label
    emit(elseLabel + ":");
    
    // Generate else branch (if exists)
    if (stmt->elseBranch) {
        visitBlock(stmt->elseBranch.get());
    }
    
    // End label
    emit(endLabel + ":");
}

void CodeGenerator::visitLoop(LoopStmt* stmt) {
    // Create labels
    std::string loopLabel = newLabel("loop");
    std::string endLabel = newLabel("end_loop");
    
    // Loop start
    emit(loopLabel + ":");
    
    // Generate condition
    visitExpr(stmt->condition.get());
    
    // Jump to end if condition is false (0)
    emit("JZ " + endLabel);
    
    // Generate body
    visitBlock(stmt->body.get());
    
    // Jump back to loop start
    emit("JMP " + loopLabel);
    
    // End label
    emit(endLabel + ":");
}

void CodeGenerator::visitBreak(BreakStmt* stmt) {
    // BREAK jumps to end of loop
    // For simplicity, we'll use a label (in a more sophisticated system,
    // we'd track the current loop's end label)
    std::string endLabel = newLabel("end_loop");
    emit("JMP " + endLabel);
    // Note: This is simplified - in production, we'd track loop context
}

void CodeGenerator::visitContinue(ContinueStmt* stmt) {
    // CONTINUE jumps to start of loop
    // For simplicity, we'll use a label
    std::string loopLabel = newLabel("loop");
    emit("JMP " + loopLabel);
    // Note: This is simplified - in production, we'd track loop context
}

void CodeGenerator::visitReturn(ReturnStmt* stmt) {
    if (stmt->value) {
        // Generate code for return value
        visitExpr(stmt->value.get());
    } else {
        // No return value, push 0
        emit("PUSH 0");
    }
    
    // Return from function
    emit("RET");
}

void CodeGenerator::visitFunction(FunctionStmt* stmt) {
    // Function label
    emit(stmt->name.lexeme + ":");
    
    // Generate function body
    visitBlock(stmt->body.get());
    
    // If no explicit return, add one
    // (In a more sophisticated system, we'd check if last statement is RET)
    emit("PUSH 0");
    emit("RET");
}

void CodeGenerator::visitBlock(BlockStmt* stmt) {
    // Generate code for all statements in block
    for (auto& stmt : stmt->statements) {
        visitStmt(stmt.get());
    }
}

// ============================================================================
// EXPRESSION CODE GENERATION
// ============================================================================

void CodeGenerator::visitExpr(Expr* expr) {
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

void CodeGenerator::visitLiteral(LiteralExpr* expr) {
    // Push literal value onto stack
    emit("PUSH " + expr->value);
}

void CodeGenerator::visitVariable(VariableExpr* expr) {
    // Load variable value onto stack
    emit("LOAD " + expr->name.lexeme);
}

void CodeGenerator::visitBinary(BinaryExpr* expr) {
    // Generate code for left operand
    visitExpr(expr->left.get());
    
    // Generate code for right operand
    visitExpr(expr->right.get());
    
    // Generate operation based on operator
    std::string op = expr->op.lexeme;
    
    if (op == "+") {
        emit("ADD");
    } else if (op == "-") {
        emit("SUB");
    } else if (op == "*") {
        emit("MUL");
    } else if (op == "/") {
        emit("DIV");
    } else if (op == "==") {
        emit("EQ");
    } else if (op == "!=") {
        // a != b: push a, push b, EQ gives (a == b), then invert
        // Stack: [a, b] -> EQ -> [result] where result is 1 if equal, 0 if not
        // We want: 1 if not equal, 0 if equal
        // So: result = 1 - (a == b)
        emit("EQ");
        emit("PUSH 1");
        emit("SUB");  // 1 - (a == b)
    } else if (op == ">") {
        emit("GT");
    } else if (op == "<") {
        emit("LT");
    } else if (op == ">=") {
        // a >= b: push a, push b, LT gives (a < b), then invert
        // Stack: [a, b] -> LT -> [result] where result is 1 if a < b, 0 otherwise
        // We want: 1 if a >= b, 0 if a < b
        // So: result = 1 - (a < b)
        emit("LT");
        emit("PUSH 1");
        emit("SUB");  // 1 - (a < b)
    } else if (op == "<=") {
        // a <= b: push a, push b, GT gives (a > b), then invert
        // Stack: [a, b] -> GT -> [result] where result is 1 if a > b, 0 otherwise
        // We want: 1 if a <= b, 0 if a > b
        // So: result = 1 - (a > b)
        emit("GT");
        emit("PUSH 1");
        emit("SUB");  // 1 - (a > b)
    } else {
        error("Unknown binary operator: " + op);
    }
}

void CodeGenerator::visitUnary(UnaryExpr* expr) {
    // Generate code for operand
    visitExpr(expr->right.get());
    
    // Generate operation based on operator
    std::string op = expr->op.lexeme;
    
    if (op == "-") {
        // Negate: 0 - x
        emit("PUSH 0");
        emit("SUB");
    } else if (op == "!") {
        // Logical NOT: 1 - x (if x is 0 or 1)
        emit("PUSH 1");
        emit("SUB");
    } else {
        error("Unknown unary operator: " + op);
    }
}

void CodeGenerator::visitCall(CallExpr* expr) {
    // Generate code for all arguments (in order)
    for (auto& arg : expr->arguments) {
        visitExpr(arg.get());
    }
    
    // Call function with argument count
    int argCount = expr->arguments.size();
    emit("CALL " + expr->callee.lexeme + " " + std::to_string(argCount));
}

