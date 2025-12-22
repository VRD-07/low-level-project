/**
 * CINEBREW Runtime Library - Header
 * Built-in functions: I/O, math, timing, and graphics stubs.
 */

#ifndef RUNTIME_H
#define RUNTIME_H

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

// Forward declaration of Window (defined in gui)
class Window;

// Represents a built-in runtime function
struct RuntimeFunction {
    std::string name;
    int paramCount;
    std::function<int(std::vector<int>&)> func;

    RuntimeFunction() : name(""), paramCount(0), func(nullptr) {}
    RuntimeFunction(const std::string& n, int params, std::function<int(std::vector<int>&)> f)
        : name(n), paramCount(params), func(f) {}
};

class Runtime {
public:
    Runtime();

    bool isBuiltin(const std::string& name) const;
    RuntimeFunction* getBuiltin(const std::string& name);
    int getParamCount(const std::string& name) const;
    int call(const std::string& name, std::vector<int>& args);

    // Set a Window reference for graphics-related builtins
    static void setWindow(Window* window);

private:
    std::unordered_map<std::string, RuntimeFunction> builtins_;

    void initializeBuiltins();

    // Built-in implementations
    static int printImpl(std::vector<int>& args);
    static int inputImpl(std::vector<int>& args);
    static int randomImpl(std::vector<int>& args);
    static int timeImpl(std::vector<int>& args);
    static int absImpl(std::vector<int>& args);
    static int minImpl(std::vector<int>& args);
    static int maxImpl(std::vector<int>& args);

    static int keyPressedImpl(std::vector<int>& args);
    static int getScreenWidthImpl(std::vector<int>& args);
    static int getScreenHeightImpl(std::vector<int>& args);

    static int clearScreenImpl(std::vector<int>& args);
    static int setColorImpl(std::vector<int>& args);
    static int drawRectangleImpl(std::vector<int>& args);
    static int drawCircleImpl(std::vector<int>& args);
    static int drawLineImpl(std::vector<int>& args);

    static Window* window_;
};

#endif // RUNTIME_H

