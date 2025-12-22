/**
 * Runtime Library Implementation
 * 
 * Provides built-in functions for CineBrew programs.
 */

#include "runtime.h"
#include "../gui/window.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

// Static window reference (set by game loop)
Window* Runtime::window_ = nullptr;

// ============================================================================
// CONSTRUCTOR
// ============================================================================

Runtime::Runtime() {
    initializeBuiltins();
}

// ============================================================================
// BUILT-IN FUNCTION IMPLEMENTATIONS
// ============================================================================

int Runtime::printImpl(std::vector<int>& args) {
    // print(value) - Print value to console
    if (args.empty()) {
        std::cout << "[EMPTY]" << std::endl;
        return 0;
    }
    std::cout << args[0] << std::endl;
    return 0;
}

int Runtime::inputImpl(std::vector<int>& args) {
    // input() - Read integer from user
    // Note: For simplicity, we'll read from stdin
    // In a real system, this would be more sophisticated
    int value;
    std::cin >> value;
    return value;
}

int Runtime::randomImpl(std::vector<int>& args) {
    // random(max) - Generate random number from 0 to max-1
    // random(min, max) - Generate random number from min to max-1
    if (args.size() == 1) {
        // random(max) - 0 to max-1
        int max = args[0];
        if (max <= 0) return 0;
        return rand() % max;
    } else if (args.size() == 2) {
        // random(min, max) - min to max-1
        int min = args[0];
        int max = args[1];
        if (max <= min) return min;
        return min + (rand() % (max - min));
    }
    return 0;
}

int Runtime::timeImpl(std::vector<int>& args) {
    // time() - Get current time in seconds since epoch
    return static_cast<int>(std::time(nullptr));
}

int Runtime::absImpl(std::vector<int>& args) {
    // abs(value) - Absolute value
    if (args.empty()) return 0;
    int value = args[0];
    return value < 0 ? -value : value;
}

int Runtime::minImpl(std::vector<int>& args) {
    // min(a, b) - Minimum of two values
    if (args.size() < 2) return 0;
    return std::min(args[0], args[1]);
}

int Runtime::maxImpl(std::vector<int>& args) {
    // max(a, b) - Maximum of two values
    if (args.size() < 2) return 0;
    return std::max(args[0], args[1]);
}

// Game-related functions (for future use)
int Runtime::keyPressedImpl(std::vector<int>& args) {
    // keyPressed(key) - Check if key is pressed
    if (args.empty() || !window_) {
        return 0;
    }
    int keyCode = args[0];
    return window_->isKeyPressed(keyCode) ? 1 : 0;
}

int Runtime::getScreenWidthImpl(std::vector<int>& args) {
    // getScreenWidth() - Get screen width
    if (window_) {
        return window_->getWidth();
    }
    return 800;  // Default screen width
}

int Runtime::getScreenHeightImpl(std::vector<int>& args) {
    // getScreenHeight() - Get screen height
    if (window_) {
        return window_->getHeight();
    }
    return 600;  // Default screen height
}

// ============================================================================
// RENDERING FUNCTIONS
// ============================================================================

int Runtime::clearScreenImpl(std::vector<int>& args) {
    // clearScreen() - Clear screen to black
    if (window_) {
        window_->clear();
    }
    return 0;
}

int Runtime::setColorImpl(std::vector<int>& args) {
    // setColor(r, g, b) - Set drawing color
    if (args.size() >= 3 && window_) {
        window_->setColor(args[0], args[1], args[2]);
    }
    return 0;
}

int Runtime::drawRectangleImpl(std::vector<int>& args) {
    // drawRectangle(x, y, w, h) - Draw filled rectangle
    if (args.size() >= 4 && window_) {
        window_->drawRectangle(args[0], args[1], args[2], args[3]);
    }
    return 0;
}

int Runtime::drawCircleImpl(std::vector<int>& args) {
    // drawCircle(x, y, radius) - Draw filled circle
    if (args.size() >= 3 && window_) {
        window_->drawCircle(args[0], args[1], args[2]);
    }
    return 0;
}

int Runtime::drawLineImpl(std::vector<int>& args) {
    // drawLine(x1, y1, x2, y2) - Draw line
    if (args.size() >= 4 && window_) {
        window_->drawLine(args[0], args[1], args[2], args[3]);
    }
    return 0;
}

// ============================================================================
// INITIALIZATION
// ============================================================================

void Runtime::initializeBuiltins() {
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Register all built-in functions
    builtins_["print"] = RuntimeFunction("print", 1, printImpl);
    builtins_["input"] = RuntimeFunction("input", 0, inputImpl);
    builtins_["random"] = RuntimeFunction("random", 1, randomImpl);
    builtins_["time"] = RuntimeFunction("time", 0, timeImpl);
    builtins_["abs"] = RuntimeFunction("abs", 1, absImpl);
    builtins_["min"] = RuntimeFunction("min", 2, minImpl);
    builtins_["max"] = RuntimeFunction("max", 2, maxImpl);
    
    // Game-related functions
    builtins_["keyPressed"] = RuntimeFunction("keyPressed", 1, keyPressedImpl);
    builtins_["getScreenWidth"] = RuntimeFunction("getScreenWidth", 0, getScreenWidthImpl);
    builtins_["getScreenHeight"] = RuntimeFunction("getScreenHeight", 0, getScreenHeightImpl);
    
    // Rendering functions
    builtins_["clearScreen"] = RuntimeFunction("clearScreen", 0, clearScreenImpl);
    builtins_["setColor"] = RuntimeFunction("setColor", 3, setColorImpl);
    builtins_["drawRectangle"] = RuntimeFunction("drawRectangle", 4, drawRectangleImpl);
    builtins_["drawCircle"] = RuntimeFunction("drawCircle", 3, drawCircleImpl);
    builtins_["drawLine"] = RuntimeFunction("drawLine", 4, drawLineImpl);
}

// ============================================================================
// BUILT-IN FUNCTION QUERIES
// ============================================================================

bool Runtime::isBuiltin(const std::string& name) const {
    return builtins_.find(name) != builtins_.end();
}

RuntimeFunction* Runtime::getBuiltin(const std::string& name) {
    auto it = builtins_.find(name);
    if (it != builtins_.end()) {
        return &it->second;
    }
    return nullptr;
}

int Runtime::getParamCount(const std::string& name) const {
    auto it = builtins_.find(name);
    if (it != builtins_.end()) {
        return it->second.paramCount;
    }
    return -1;
}

int Runtime::call(const std::string& name, std::vector<int>& args) {
    RuntimeFunction* func = getBuiltin(name);
    if (!func) {
        return 0;  // Function not found
    }
    return func->func(args);
}

void Runtime::setWindow(Window* window) {
    window_ = window;
}

