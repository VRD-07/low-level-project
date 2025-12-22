/**
 * CINEBREW Virtual Machine - Header
 * Stack-based bytecode interpreter interface.
 */

#ifndef VM_H
#define VM_H

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "../runtime/runtime.h"

// Call frame for function calls
struct Frame {
    int return_pc;
    int prev_stack_size;
    int arg_count;
};

class VM {
public:
    std::vector<int> stack;
    std::unordered_map<std::string, int> vars;
    int pc;
    std::unordered_map<std::string, int> labels;
    std::vector<Frame> callstack;
    Runtime runtime;

    VM();

    void push(int value);
    int pop();

    void preprocess(std::vector<std::string>& program);
    void execute(const std::string& instruction, std::vector<std::string>& program);
    void run(std::vector<std::string> program);

    void printStack() const;
    void printVars() const;
    void reset();

private:
    std::vector<std::string> split(const std::string& s);
};

#endif // VM_H

