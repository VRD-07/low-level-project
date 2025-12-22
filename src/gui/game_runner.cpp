#include "../compiler/compiler.h"
#include "../vm/vm.h"
#include "game_loop.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: game_runner <source_file.cb> [width height]" << std::endl;
        return 1;
    }

    const char* path = argv[1];
    int width = 800;
    int height = 600;
    if (argc >= 4) {
        int w = std::atoi(argv[2]);
        int h = std::atoi(argv[3]);
        if (w > 0 && h > 0) {
            width = w; height = h;
        }
    }

    // Read source file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    file.close();

    std::cout << "Compiling game: " << path << std::endl;

    // Compile
    Compiler compiler;
    std::vector<std::string> bytecode;
    try {
        bytecode = compiler.compile(source);
    } catch (const std::exception& ex) {
        std::cerr << "Compilation failed with exception: " << ex.what() << std::endl;
        return 1;
    }

    if (compiler.hadError()) {
        std::cerr << "\nCompilation Errors:" << std::endl;
        for (const auto& error : compiler.getErrors()) {
            std::cerr << "  " << error << std::endl;
        }
        return 1;
    }

    if (bytecode.empty()) {
        std::cerr << "Compilation produced no bytecode." << std::endl;
        return 1;
    }

    std::cout << "Compilation successful!" << std::endl;
    std::cout << "Starting game... (" << width << "x" << height << ")" << std::endl << std::endl;

    // Create VM and load bytecode
    VM vm;
    try {
        // Preprocess labels and initialize VM state if available
        vm.preprocess(bytecode);
        vm.run(bytecode); // run top-level initialization if needed
    } catch (const std::exception& ex) {
        std::cerr << "Runtime error during VM initialization: " << ex.what() << std::endl;
        return 1;
    }

    // Create and run game loop
    GameLoop gameLoop(vm, bytecode, width, height, "CineBrew Game");
    gameLoop.setTargetFPS(60);

    if (!gameLoop.isRunning()) {
        std::cerr << "Game loop failed to initialize (window or runtime not available)." << std::endl;
        return 1;
    }

    try {
        gameLoop.run();
    } catch (const std::exception& ex) {
        std::cerr << "Unhandled exception in game loop: " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "Game ended." << std::endl;

    return 0;
}

