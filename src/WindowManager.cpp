#include "WindowManager.h"

#include <mutex>
#include <print>

#include "Window.h"

static int s_winNum = 0;
static std::mutex s_winMtx;

// ----- OpenGL ----- Functions -----

static void initGLFW() {
    // Only initialize once
    if (s_winNum != 0) {
        return;
    }
    
    // Init glfw
    if (!glfwInit()) {
        std::println(stderr, "ERROR: Could not initialize GLFW.");
        exit(EXIT_FAILURE);
    }

    // GLFW v3.3, CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void closeGLFW() {
    if (s_winNum != 0) {
        return;
    }

    glfwTerminate();
}

void WindowManager::add() {
    std::lock_guard lock(s_winMtx);
    initGLFW();
    s_winNum++;
}

void WindowManager::remove() {
    std::lock_guard lock(s_winMtx);
    s_winNum--;
    closeGLFW();
}

bool WindowManager::hasWindows() {
    return s_winNum;
}

