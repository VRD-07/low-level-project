/**
 * Game Loop Implementation
 * 
 * Implements the main game loop that calls CineBrew functions.
 */

#include "game_loop.h"
#include "../runtime/runtime.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// ============================================================================
// CONSTRUCTOR
// ============================================================================

GameLoop::GameLoop(VM& vm, const std::vector<std::string>& program,
                   int width, int height, const std::string& title) 
    : vm_(vm), program_(program), running_(true), targetFPS_(60) {
    
    // Initialize window
    if (Window::initialize()) {
        window_ = std::make_unique<Window>(width, height, title);
        
        // Set window reference in runtime (for rendering functions)
        Runtime::setWindow(window_.get());
    } else {
        std::cerr << "Failed to initialize window" << std::endl;
        running_ = false;
    }
}

// ============================================================================
// MAIN GAME LOOP
// ============================================================================

void GameLoop::run() {
    if (!window_ || !window_->isOpen()) {
        std::cerr << "Window not available, cannot run game loop" << std::endl;
        return;
    }
    
    std::cout << "Starting game loop at " << targetFPS_ << " FPS..." << std::endl;
    
    const int frameTimeMs = 1000 / targetFPS_;  // Time per frame in milliseconds
    
    while (running_ && window_->isOpen()) {
        long long frameStart = getCurrentTimeMs();
        
        // 1. Process input (and window events)
        if (!window_->processEvents()) {
            // Window closed
            stop();
            break;
        }
        processInput();
        
        // 2. Update game state
        update();
        
        // 3. Render graphics
        render();
        
        // 4. Present frame
        window_->present();
        
        // 5. Frame timing (maintain target FPS)
        long long frameEnd = getCurrentTimeMs();
        long long frameDuration = frameEnd - frameStart;
        long long sleepTime = frameTimeMs - frameDuration;
        
        if (sleepTime > 0) {
            sleep(static_cast<int>(sleepTime));
        }
    }
    
    std::cout << "Game loop stopped." << std::endl;
}

void GameLoop::stop() {
    running_ = false;
}

// ============================================================================
// GAME LOOP COMPONENTS
// ============================================================================

void GameLoop::processInput() {
    // Input is processed in window_->processEvents()
    // Key state is available through window_->isKeyPressed()
    // CineBrew programs can check keys using keyPressed() function
}

void GameLoop::update() {
    // Call CineBrew update() function if it exists
    callCineBrewFunction("update");
}

void GameLoop::render() {
    // Call CineBrew render() function if it exists
    callCineBrewFunction("render");
}

// ============================================================================
// CINEBREW FUNCTION CALLING
// ============================================================================

bool GameLoop::callCineBrewFunction(const std::string& functionName) {
    // Check if function exists in VM labels
    if (vm_.labels.find(functionName) == vm_.labels.end()) {
        // Function doesn't exist - that's okay, just skip
        return false;
    }
    
    // Save current VM state
    int savedPC = vm_.pc;
    int savedStackSize = vm_.stack.size();
    
    // Create a CALL instruction to call the function
    std::string callInstr = "CALL " + functionName + " 0";
    
    // Execute the CALL instruction
    vm_.execute(callInstr, program_);
    
    // Execute the function until it returns
    int maxInstructions = 1000;  // Safety limit
    int instructionCount = 0;
    
    while (instructionCount < maxInstructions && vm_.pc < (int)program_.size()) {
        // Check if we've returned (callstack is empty and we're past the function)
        if (vm_.callstack.empty() && vm_.pc > vm_.labels[functionName]) {
            break;
        }
        
        vm_.execute(program_[vm_.pc], program_);
        instructionCount++;
    }
    
    // Restore PC to saved position
    vm_.pc = savedPC;
    
    // Keep variables but restore stack size (function may have left return value)
    // For update/render functions, we don't care about return values
    if (vm_.stack.size() > savedStackSize) {
        vm_.stack.resize(savedStackSize);
    }
    
    return true;
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void GameLoop::sleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);  // usleep takes microseconds
#endif
}

long long GameLoop::getCurrentTimeMs() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

