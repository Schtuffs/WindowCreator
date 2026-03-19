#pragma once

#include <functional>
#include <iostream>
#include <mutex>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Colour.h"
#include "Renderable.h"

/**
 * @class Window
 * @brief Main class for creating windows
 * @author Kyle Wagler
 * @date 2026-03-19
 */
class Window {
public:
    // ----- Creation ----- Destruction -----

    Window(const std::string& title = "Window");
    /**
     * @brief Allows for checking if the window was properly created.
     * Allows use only if the window was properly created and initialized. Calling set functions without checking this is undefined behaviour.
     * @return true on working window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool isCreated();
    ~Window();
    
    /**
     * @brief Allows for checking if main window is closed.
     * This function waits for the main window to close before returning control to developer.
     * @param lambda a function to run on each frame loop.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    static void await(const std::function<void(void)>& lambda = [](){});


    
    // ----- Setters -----
    
    /**
     * @brief Sets this window as the main window. First successfully created window is automatically the main.
     * On main window close, all other windows are also closed. This allows for sub windows to be properly closed when the initial is closed.
     * The user can continue program after Window::await
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setMain();
    
    /**
     * @brief Changes the background colour of the window.
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setBackground(Colour col);
    
    /**
     * @brief Changes the size of the window
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setSize(int width, int height);
    
    /**
     * @brief Changes the title of the window
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setTitle(const std::string& title);
    
    // Chooses callback overload based on passed param
    
    /**
     * @brief Sets a callback for keypressed
     * @param callback the callback to set with format <code>callback(GLFWwindow* window, int key, int scancode, int action, int mods)</code>
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWkeyfun callback);
    /**
     * @brief Sets a callback for cursor position
     * @param callback the callback to set with format <code>callback(GLFWwindow* window, double xpos, double ypos)</code>
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWcursorposfun callback);
    /**
     * @brief Sets a callback for mouse button press
     * @param callback the callback to set with format <code>callback(GLFWwindow* window, int button, int action, int mods)</code>
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWmousebuttonfun callback);
    /**
     * @brief Sets a callback for chars (supports unicode)
     * @param callback the callback to set with format <code>callback(GLFWwindow* window, unsigned int codepoint)</code>
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWcharfun callback);
    /**
     * @brief Sets a callback for window size changes
     * @param callback the callback to set with format <code>callback(GLFWwindow* window, int width, int height)</code>
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWframebuffersizefun callback);
    /**
     * @brief Sets a callback for window redraw (such as window covered)
     * @param callback the callback to set with format <code>callback(GLFWwindow* window)</code>
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& setCallback(GLFWwindowrefreshfun callback);
    
    /**
     * @brief Adds a new renderable object to window
     * @param obj the new <code>Renderable</code> object
     * @return the updated window
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    Window& add(Renderable& obj);
    /**
     * @brief Removes a <code>Renderable</code> object from the window.
     * @param index the index of the <code>Renderable</code> object to remove from window.
     * @return true on successful removal.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool remove(uint64_t index);
    /**
     * @brief Removes a <code>Renderable</code> object from the window.
     * @param obj the <code>Renderable</code> object to remove from window.
     * @return true on successful removal.
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    bool remove(Renderable& obj);
    
    /**
     * @brief Runs window with current args
     * @param frames Frames per Second, 0 for no limit
     * @param upsMatchFps if the Updated per Second should match the Frames per Second
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    void run(uint64_t frames = 60, bool upsMatchFps = true);
    
    
    
    // ----- Getters -----
    
    /**
     * @brief Returns the list of all the renderable items
     * @return list of all renderables
     * @author Kyle Wagler
     * @date 2026-03-19
     */
    std::vector<Renderable&> renderables();

private:
    // Holds the window data
    GLFWwindow* m_window;
    bool m_created, m_closed;
    int m_width, m_height;

    // Updates window size
    std::mutex m_sizeLock;
    bool m_sizeChanged;

    // Rendering objects
    std::vector<Renderable*> m_renderObjects;

    // Time since previous render
    uint64_t m_prevTime, m_frameTime;

    // For rendering frames at defined frame rate
    bool hasFrameTimePassed(bool upsMatchFps);
    
    // Required OpenGL functions for a window to function

    // Main window loop
    void showWindow(bool upsMatchFps);

    // Main function to check for looping
    bool shouldClose();

    // Checks and updates window for any changes
    void checkChanges();

    // Checks the size changes
    void checkSizeChange();

    // Render window
    void render(bool upsMatchFps);

    // On close
    void close();
};

