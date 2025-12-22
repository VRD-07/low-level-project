/**
 * AST Implementation
 * String representations for debugging
 */

#include "ast.h"
#include <sstream>

// ============================================================================
// EXPRESSION TO STRING
// ============================================================================

std::string LiteralExpr::toString() const {
    return value;
}

std::string VariableExpr::toString() const {
    return name.lexeme;
}

std::string BinaryExpr::toString() const {
    return "(" + left->toString() + " " + op.lexeme + " " + right->toString() + ")";
}

std::string UnaryExpr::toString() const {
    return "(" + op.lexeme + right->toString() + ")";
}

std::string CallExpr::toString() const {
    std::string result = callee.lexeme + "(";
    for (size_t i = 0; i < arguments.size(); i++) {
        if (i > 0) result += ", ";
        result += arguments[i]->toString();
    }
    result += ")";
    return result;
}

// ============================================================================
// STATEMENT TO STRING
// ============================================================================

std::string DeclarationStmt::toString() const {
    std::string result = keyword.lexeme + " " + name.lexeme + " = ";
    if (initializer) {
        result += initializer->toString();
    }
    result += ";";
    return result;
}

std::string AssignmentStmt::toString() const {
    return name.lexeme + " = " + value->toString() + ";";
}

std::string ExpressionStmt::toString() const {
    return expression->toString() + ";";
}

std::string PrintStmt::toString() const {
    return keyword.lexeme + " " + expression->toString() + ";";
}

std::string IfStmt::toString() const {
    std::string result = keyword.lexeme + " " + condition->toString() + " ";
    result += thenBranch->toString();
    if (elseBranch) {
        result += " " + keyword.lexeme + " { ... }";  // Simplified
    }
    return result;
}

std::string LoopStmt::toString() const {
    return keyword.lexeme + " " + condition->toString() + " " + body->toString();
}

std::string BreakStmt::toString() const {
    return keyword.lexeme + ";";
}

std::string ContinueStmt::toString() const {
    return keyword.lexeme + ";";
}

std::string ReturnStmt::toString() const {
    std::string result = keyword.lexeme;
    if (value) {
        result += " " + value->toString();
    }
    result += ";";
    return result;
}

std::string BlockStmt::toString() const {
    std::string result = "{\n";
    for (const auto& stmt : statements) {
        result += "  " + stmt->toString() + "\n";
    }
    result += "}";
    return result;
}

std::string FunctionStmt::toString() const {
    std::string result = keyword.lexeme + " " + name.lexeme + "(";
    for (size_t i = 0; i < parameters.size(); i++) {
        if (i > 0) result += ", ";
        result += parameters[i].lexeme;
    }
    result += ") " + body->toString();
    return result;
}

// ============================================================================
// PROGRAM TO STRING
// ============================================================================

std::string Program::toString() const {
    std::string result = "Program:\n";
    for (const auto& stmt : statements) {
        result += stmt->toString() + "\n";
    }
    return result;
}

