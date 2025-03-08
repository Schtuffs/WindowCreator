#include "WindowManager.h"

#include <mutex>

#include "Window.h"

static bool s_initialized = false;
static int s_winNum = 0;
static std::mutex s_addlock, s_removelock;

// ----- OpenGL ----- Functions -----

static void initGLFW() {
    // Only initialize once
    if (s_initialized) {
        return;
    }
    
    // Init glfw
    if (!glfwInit()) {
        std::cout << "Could not initialize glfw..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // GLFW v3.3, CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tracks OpenGL initialized
    s_initialized = true;
}

static void closeGLFW() {
    if (s_initialized && s_winNum <= 0) {
        glfwTerminate();
        s_initialized = false;
    }
}

void WindowManager::add() {
    std::lock_guard lock(s_addlock);
    s_winNum++;
    initGLFW();
}

void WindowManager::remove() {
    std::lock_guard lock(s_removelock);
    s_winNum--;
    closeGLFW();
}

bool WindowManager::hasWindows() {
    return s_winNum;
}

