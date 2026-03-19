#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Colour.h"
#include "Window.h"
#include "Callbacks.h"

int main(void) {
    // Create and check main window
    Window window("Window 1");
    if (!window.isCreated()) {
        return 0;
    }
    window.setSize(800, 800)
        .setBackground(Colour(0, 0, 0))
        .setCallback(key_callback_main)
        .run(0, false);
    Window window2("Window 2");
    if (window2.isCreated()) {
        window2.setSize(800, 800)
           .setBackground(Colour(255, 0, 0))
           .setCallback(key_callback_alt)
           .run();
    }
    Window window3("Window 3");
    if (window3.isCreated()) {
        window3.setSize(800, 800)
           .setBackground(Colour(0, 255, 0))
           .setCallback(key_callback_main)
           .run();
    }
    Window window4("Window 4");
    if (window4.isCreated()) {
        window4.setSize(800, 800)
           .setBackground(Colour(0, 0, 255))
           .setCallback(key_callback_alt)
           .run();
    }

    int num = 0;
    Window::await([&]() {
        window.setTitle(std::to_string(num));
        num++;
    });
    return 0;
}

