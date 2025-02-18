#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Colour.h"
#include "Renderable.h"

class WindowManager {
private:
    // Holds values for opening/closing OpenGL
    static bool s_initialized;
    static unsigned short s_totalWindows;

    GLFWwindow* m_window;
    bool m_created;

    std::vector<Renderable*> m_renderObjects;
    double m_prevTime, m_frameTime;

    // Initializes glfw
    void init();
    // Closes OpenGL
    void close();

    // For rendering frames at defined frame rate
    bool hasFrameTimePassed();

public:
    // Creation/Destruction
    WindowManager(const std::string& title = "Window");
    bool isCreated();
    ~WindowManager();

    // Setters

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

    // Required OpenGL functions for a window to function

    // Main function to check for looping
    bool shouldClose();

    // Poll for actions on window
    void poll();

    // Render window
    void render();
};

