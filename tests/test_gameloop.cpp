/**
 * Game Loop Test Program
 * 
 * Tests the game loop with a simple CineBrew program.
 */

#include "../src/compiler/compiler.h"
#include "../src/vm/vm.h"
#include "../src/gui/game_loop.h"
#include <iostream>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   CINEBREW Game Loop Test" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Simple game program
    std::string source = R"(
TAKE x = 100;
TAKE y = 100;
TAKE velocityX = 2;
TAKE velocityY = 2;

SCENE update() {
    x = x + velocityX;
    y = y + velocityY;
    
    IF x < 0 {
        velocityX = abs(velocityX);
    }
    IF x > getScreenWidth() {
        velocityX = -abs(velocityX);
    }
    IF y < 0 {
        velocityY = abs(velocityY);
    }
    IF y > getScreenHeight() {
        velocityY = -abs(velocityY);
    }
    
    SHOT 0;
}

SCENE render() {
    POUR x;
    POUR y;
    SHOT 0;
}
)";
    
    std::cout << "Source code:" << std::endl;
    std::cout << source << std::endl;
    std::cout << std::endl;
    
    // Compile
    Compiler compiler;
    std::vector<std::string> bytecode = compiler.compile(source);
    
    if (compiler.hadError()) {
        std::cout << "Compilation Errors:" << std::endl;
        for (const auto& error : compiler.getErrors()) {
            std::cout << "  " << error << std::endl;
        }
        return 1;
    }
    
    std::cout << "Compilation successful!" << std::endl;
    std::cout << std::endl;
    
    // Create VM and load bytecode
    VM vm;
    vm.preprocess(bytecode);
    
    // Initialize game state (set initial values)
    vm.vars["x"] = 100;
    vm.vars["y"] = 100;
    vm.vars["velocityX"] = 2;
    vm.vars["velocityY"] = 2;
    
    std::cout << "Running game loop..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // Create and run game loop
    GameLoop gameLoop(vm, bytecode);
    gameLoop.setTargetFPS(10);  // 10 FPS for testing (slower)
    gameLoop.run();
    
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Game loop completed!" << std::endl;
    std::cout << std::endl;
    
    // Print final state
    std::cout << "Final game state:" << std::endl;
    vm.printVars();
    
    return 0;
}

