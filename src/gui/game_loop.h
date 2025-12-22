/**

/**
 * CINEBREW Game Loop - Header
 * Update/render cycle with frame timing control.
 */

#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "../vm/vm.h"
#include "window.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>

class GameLoop {
public:
    GameLoop(VM& vm, const std::vector<std::string>& program, int width = 800, int height = 600, const std::string& title = "CineBrew Game");
    void run();
    void stop();
    bool isRunning() const { return running_; }
    void setTargetFPS(int fps) { targetFPS_ = fps; }

private:
    VM& vm_;
    std::vector<std::string> program_;
    std::unique_ptr<Window> window_;
    bool running_;
    int targetFPS_;

    bool callCineBrewFunction(const std::string& functionName);
    void processInput();
    void update();
    void render();
    void sleep(int milliseconds);
    long long getCurrentTimeMs();
};

#endif // GAME_LOOP_H
    

    // Call a CineBrew function by name
