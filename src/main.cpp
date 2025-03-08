#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Colour.h"
#include "Window.h"
#include "Callbacks.h"

int main(void) {
    // Create and check main window
    Window first("Example");
    if (!first.isCreated()) {
        std::cout << "The window failed to be created.\n";
        return 0;
    }
    
    // Window setup
    first.setBackground(Colour(0, 0, 0));
    
    // Set a simple callback
    first.setCallback(key_callback_main);
    
    // Run the window
    first.run();
    
    Window second("Second");
    second.setBackground(Colour(255,0,0));
    second.setCallback(key_callback_main);
    second.run();
    
    Window third("Third");
    third.setBackground(Colour(0,255,0));
    third.setCallback(key_callback_alt);
    third.run();
    
    Window fourth("Fourth");
    fourth.setBackground(Colour(255,0,255));
    fourth.setCallback(key_callback_alt);
    fourth.run();
    
    Window::await();
    return 0;
}

