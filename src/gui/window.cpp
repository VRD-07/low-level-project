/**

/**
 * Window Implementation - CONSOLE BACKEND (STUB)
 *
 * This is a non-functional graphics stub that prints debug output instead of rendering.
 * The window interface is complete and ready for SDL2 integration.
 * Games can compile and run with text output only.
 */
#include "window.h"
#include <iostream>
#include <cstring>

// Static initialization: no-op for stub
bool Window::initialize() {
    std::cout << "[GUI] initialize (stub)" << std::endl;
    return true;
}

void Window::shutdown() {
    std::cout << "[GUI] shutdown (stub)" << std::endl;
}

Window::Window(int width, int height, const std::string& title)
    : window_(nullptr), renderer_(nullptr), width_(width), height_(height), isOpen_(true) {
    std::cout << "[GUI] Created window: " << title << " (" << width << "x" << height << ") (stub)" << std::endl;
    std::memset(keys_, 0, sizeof(keys_));
}

Window::~Window() {
    std::cout << "[GUI] Destroyed window (stub)" << std::endl;
}

void Window::close() {
    isOpen_ = false;
}

void Window::clear() {
    // Stub: no-op
}

void Window::present() {
    // Stub: no-op
}

void Window::setColor(int r, int g, int b) {
    // Stub: no-op
}

void Window::drawRectangle(int x, int y, int w, int h) {
    std::cout << "[GUI] Draw rectangle: (" << x << ", " << y << ") " << w << "x" << h << " (stub)" << std::endl;
}

void Window::drawCircle(int x, int y, int radius) {
    std::cout << "[GUI] Draw circle: (" << x << ", " << y << ") r=" << radius << " (stub)" << std::endl;
}

void Window::drawLine(int x1, int y1, int x2, int y2) {
    std::cout << "[GUI] Draw line: (" << x1 << ", " << y1 << ") -> (" << x2 << ", " << y2 << ") (stub)" << std::endl;
}

bool Window::processEvents() {
    // Stub: nothing to process; keep window open until closed
    return isOpen_;
}

bool Window::isKeyPressed(int keyCode) const {
    if (keyCode < 0 || keyCode >= 256) return false;
    return keys_[keyCode];
}

