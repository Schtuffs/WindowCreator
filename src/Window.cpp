#include "Window.h"

#include <thread>

#include "WindowManager.h"

// Holds values for opening/closing OpenGL
static bool s_forceClose = false;
// Holds a specified main window
static GLFWwindow* s_mainWindow = nullptr;
// Handle to all windows
static std::vector<Window> s_allWindows;

// ----- Creation ----- Destruction -----

Window::Window(const std::string& title) {
    // Add window to count
    WindowManager::add();

    // Create the window
    this->m_window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, title.c_str(), NULL, NULL);
    if (this->m_window == nullptr) {
        std::cout << "Could not create window..." << std::endl;
        this->m_created = false;

        // Remove failed window
        WindowManager::remove();
        return;
    }

    this->m_created = true;
    glfwMakeContextCurrent(this->m_window);
    if (s_mainWindow == nullptr) {
        s_mainWindow = this->m_window;
    }
    
    // Glad initialization
    gladLoadGL();
    glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

    // Setting blendmode so .png have blank backgrounds
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable vsync
    glfwSwapInterval(0);

    // Other variables
    this->m_prevTime = 0;
    this->m_frameTime = 0;
    s_allWindows.push_back(*this);
}

bool Window::isCreated() {
    return this->m_created;
}

Window::~Window() {
    // Nothing todo
}

void Window::close() {
    if (this->m_window != nullptr) {
        if (this->m_window == s_mainWindow) {
            s_mainWindow = nullptr;
        }
        
        glfwDestroyWindow(this->m_window);
        this->m_window = nullptr;
    }
}

void Window::await() {
    // Return if no main
    if (s_mainWindow == nullptr) {
        return;
    }
    
    // Wait until main is closed
    while (WindowManager::hasWindows() && !s_forceClose) {
        glfwPollEvents();
        
        // Check all windows are open
        size_t q = s_allWindows.size();
        for (size_t i = 0; i < q; i++) {
            if (s_allWindows[i].shouldClose()) {
                s_allWindows[i].close();
                s_allWindows.erase(s_allWindows.begin() + i);
                WindowManager::remove();
            }
        }
    }

    // Close other windows in case
    for (Window& w : s_allWindows) {
        w.close();
    }
    s_allWindows.clear();
}



// ----- Setters -----

void Window::setMain() {
    s_mainWindow = this->m_window;
}

void Window::setBackground(Colour col) {
    float r = col.getR() * (1. / COLOUR_MAX);
    float g = col.getG() * (1. / COLOUR_MAX);
    float b = col.getB() * (1. / COLOUR_MAX); 
    float a = col.getA() * (1. / COLOUR_MAX);
    glfwMakeContextCurrent(this->m_window);
    glClearColor(r, g, b, a);
}

void Window::add(Renderable& obj) {
    this->m_renderObjects.push_back(&obj);
}



// ----- Callbacks -----

void Window::setCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(this->m_window, callback);
}

void Window::setCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->m_window, callback);
}

void Window::setCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->m_window, callback);
}

void Window::setCallback(GLFWcharfun callback) {
    glfwSetCharCallback(this->m_window, callback);
}

void Window::setCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->m_window, callback);
}

void Window::setCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(this->m_window, callback);
}



// ----- Read -----

bool Window::shouldClose() {
    // If this is the main window, check if its closed
    if (s_mainWindow != nullptr && this->m_window == s_mainWindow) {
        s_forceClose = glfwWindowShouldClose(this->m_window);
        return s_forceClose;
    }

    return glfwWindowShouldClose(this->m_window);
}

void Window::render() {
    // Check time since last frame
    if (!this->hasFrameTimePassed()) {
        return;
    }
    
    // Set background
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render objects
    for (Renderable* obj : this->m_renderObjects) {
        obj->Render();
    }

    glfwSwapBuffers(this->m_window);
}

bool Window::hasFrameTimePassed() {
    // If no specified rendering fps
    if (this->m_frameTime == 0) {
        return true;
    }
    
    double current = clock();
    double deltaTime = this->m_frameTime - (double)(current - this->m_prevTime);
    if (0 >= deltaTime) {
        // Only renders and swaps buffers at WINDOW_MAX_FPS to save GPU
        // Allows for other calculations to happen more
        
        // Accounts for extra offset of frame times
        // If the frame took an extra 0.01 clocks,
        // The current time is set back by that 0.01 to allow
        // For proper counting of frames
        // Ex: s_prevTime = 1800 + (-0.23);
        // s_prevTime = 1799.77
        this->m_prevTime = current + deltaTime;
        return true;
    }
    return false;
}



// ----- Update -----

void Window::run() {
    // Remove context to set window in thread
    glfwMakeContextCurrent(nullptr);

    // Put onto thread
    std::thread t(&Window::showWindow, this);
    t.detach();
}

void Window::showWindow() {
    std::cout << "Thread made on " << std::this_thread::get_id() << "\n";

    // Set window on this thread
    glfwMakeContextCurrent(this->m_window);

    // Loop while window is open
    while (!this->shouldClose() && !s_forceClose) {
        this->render();
    }

    // Window is closed
    glfwMakeContextCurrent(nullptr);
}

