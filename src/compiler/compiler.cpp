/**
 * Compiler Implementation
 * 
 * Orchestrates all compilation stages.
 */

#include "compiler.h"
#include <iostream>

Compiler::Compiler() : hadError_(false) {
    // Lexer, Parser, SemanticAnalyzer, and CodeGenerator
    // are created on-demand in compile() method
}

std::vector<std::string> Compiler::compile(const std::string& source) {
    errors_.clear();
    bytecode_.clear();
    hadError_ = false;
    
    // Stage 1: Lexing
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    if (lexer.hadError()) {
        errors_.push_back("Lexer: " + lexer.getError());
        hadError_ = true;
        return bytecode_;
    }
    
    // Stage 2: Parsing
    Parser parser(tokens);
    std::unique_ptr<Program> program = parser.parse();
    if (parser.hadError()) {
        errors_.push_back("Parser: " + parser.getError());
        hadError_ = true;
        return bytecode_;
    }
    
    // Stage 3: Semantic Analysis
    SemanticAnalyzer analyzer;
    analyzer.analyze(program);
    if (analyzer.hadError()) {
        errors_ = analyzer.getErrors();
        hadError_ = true;
        return bytecode_;
    }
    
    // Stage 4: Code Generation
    CodeGenerator codegen;
    bytecode_ = codegen.generate(program);
    if (codegen.hadError()) {
        errors_.push_back("CodeGen: " + codegen.getError());
        hadError_ = true;
        return bytecode_;
    }
    
    return bytecode_;
}

bool Compiler::hadError() const {
    return hadError_;
}

std::vector<std::string> Compiler::getErrors() const {
    return errors_;
}

std::vector<std::string> Compiler::getBytecode() const {
    return bytecode_;
}

