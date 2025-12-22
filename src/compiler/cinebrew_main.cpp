// Minimal CLI entrypoint for CineBrew
// Usage:
//   cinebrew run <file>
//   cinebrew <file>

#include "compiler.h"
#include "../vm/vm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

static void printUsage() {
    std::cerr << "Usage:\n  cinebrew run <file>\n  cinebrew <file>\n";
}

int main(int argc, char* argv[]) {
    if (argc == 2 || (argc == 3 && std::string(argv[1]) == "run")) {
        const char* path = (argc == 2) ? argv[1] : argv[2];

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Error: could not open file: " << path << std::endl;
            return 1;
        }

        std::stringstream buf;
        buf << file.rdbuf();
        std::string source = buf.str();
        file.close();

        std::cout << "Compiling: " << path << std::endl;

        Compiler compiler;
        std::vector<std::string> bytecode;
        try {
            bytecode = compiler.compile(source);
        } catch (const std::exception& ex) {
            std::cerr << "Compilation failed: " << ex.what() << std::endl;
            return 1;
        }

        if (compiler.hadError()) {
            std::cerr << "Compilation errors:\n";
            for (const auto& e : compiler.getErrors()) std::cerr << "  " << e << std::endl;
            return 1;
        }

        if (bytecode.empty()) {
            std::cerr << "No bytecode produced." << std::endl;
            return 1;
        }

        std::cout << "Running..." << std::endl;
        VM vm;
        try {
            vm.run(bytecode);
        } catch (const std::exception& ex) {
            std::cerr << "Runtime error: " << ex.what() << std::endl;
            return 1;
        }

        return 0;
    }

    printUsage();
    return 1;
}

