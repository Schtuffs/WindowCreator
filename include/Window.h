#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Colour.h"
#include "Renderable.h"

class Window {
private:
    // Holds the window data
    GLFWwindow* m_window;
    bool m_created;

    // Rendering objects
    std::vector<Renderable*> m_renderObjects;

    // Time since previous render
    double m_prevTime, m_frameTime;

    // For rendering frames at defined frame rate
    bool hasFrameTimePassed();
    
    // Required OpenGL functions for a window to function

    // Main window loop
    void showWindow();

    // Main function to check for looping
    bool shouldClose();

    // Render window
    void render();

    // On close
    void close();

public:
    // ----- Creation ----- Destruction -----

    Window(const std::string& title = "Window");
    bool isCreated();
    ~Window();

    // Returns when main window is closed
    static void await();



    // ----- Setters -----

    // Sets this windows as the main window
    // If this window is closed, program ends
    void setMain();

    // Set background to specified colour
    void setBackground(Colour col);

    // Chooses callback overload based on passed param

    // Sets the key callback for the window
    void setCallback(GLFWkeyfun callback);
    // Sets the cursor position callback for the window
    void setCallback(GLFWcursorposfun callback);
    // Sets the mouse button callback for the window
    void setCallback(GLFWmousebuttonfun callback);
    // Sets the character pressed callback for the window
    void setCallback(GLFWcharfun callback);
    // Sets the window resize callback for the window
    void setCallback(GLFWframebuffersizefun callback);
    // Sets the window refresh callback for the window
    void setCallback(GLFWwindowrefreshfun callback);

    // Add a new drawable into the screen
    void add(Renderable& obj);

    // Runs window with current args
    void run();
};

