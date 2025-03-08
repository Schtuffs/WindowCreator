#include "Callbacks.h"

#include "Window.h"

void key_callback_main(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void key_callback_alt(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Window* win = new Window("Callback");
        if (!win->isCreated()) {
            return;
        }
        win->setBackground(Colour(rand()%255,rand()%255,rand()%255));
        win->setCallback(key_callback_main);
        win->run();
    }
}

