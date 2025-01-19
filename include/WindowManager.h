#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Colour.h"
#include "Renderable.h"

class WindowManager {
private:
    GLFWwindow* m_window;
    bool m_created;

    std::vector<Renderable*> m_renderObjects;
    double m_prevTime, m_frameTime;

    // For rendering frames at defined frame rate
    bool HasFrameTimePassed();

public:
    // Creation/Destruction
    WindowManager(const std::string& title = "Window");
    bool IsCreated();
    ~WindowManager();

    // Setters

    // Set background to specified colour
    void SetBackground(Colour col);

    // Chooses callback overload based on passed param

    // Sets the key callback for the window
    void SetCallback(GLFWkeyfun callback);
    // Sets the cursor position callback for the window
    void SetCallback(GLFWcursorposfun callback);
    // Sets the mouse button callback for the window
    void SetCallback(GLFWmousebuttonfun callback);
    // Sets the character pressed callback for the window
    void SetCallback(GLFWcharfun callback);
    // Sets the window resize callback for the window
    void SetCallback(GLFWframebuffersizefun callback);
    // Sets the window refresh callback for the window
    void SetCallback(GLFWwindowrefreshfun callback);

    // Add a new drawable into the screen
    void Add(Renderable& obj);

    // Required OpenGL functions for a window to function

    // Main function to check for looping
    bool ShouldClose();

    // Poll for actions on window
    void Poll();

    // Render window
    void Render();
};

