#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Colour.h"
#include "WindowManager.h"
#include "Callbacks.h"

// Does simple initialization functions
// Will automatically end program if anything goes wrong
void init() {
    // Init glfw
    if (!glfwInit()) {
        std::cout << "Could not initialize glfw..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // GLFW v3.3, CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// Closes OpenGL
void close() {
    glfwTerminate();
}

int main(void) {
    // OpenGL initialization
    init();

    // Create and check main window
    WindowManager window("Example");
    if (!window.IsCreated()) {
        std::cout << "The window failed to be created.\n";
        close();
        exit(0);
    }

    // Window setup
    window.SetBackground(Colour(0, 0, 0));

    // Set a simple callback
    window.SetCallback(key_callback_main);

    // Main window loop
    Colour col(0, 0, 0);
    while (!window.ShouldClose()) {
        window.Poll();
        window.Render();
    }

    // Cleanup functions
    close();
    return 0;
}

