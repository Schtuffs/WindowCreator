#include "WindowManager.h"

// Creation / Destruction

WindowManager::WindowManager(const std::string& title) {
    // Create the window
    this->m_window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, title.c_str(), NULL, NULL);
    if (this->m_window == nullptr) {
        std::cout << "Could not create window..." << std::endl;
        this->m_created = false;
        return;
    }

    this->m_created = true;
    glfwMakeContextCurrent(this->m_window);
    
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
    this->m_frameTime = CLOCKS_PER_SEC / WINDOW_MAX_FPS;
}

bool WindowManager::IsCreated() {
    return this->m_created;
}

WindowManager::~WindowManager() {
    // Free all alocated memory
    if (this->m_window != nullptr) {
        glfwDestroyWindow(this->m_window);
    }
}



// ----- Setters -----

void WindowManager::SetBackground(Colour col) {
    float r = col.GetR() * (1. / 255);
    float g = col.GetG() * (1. / 255);
    float b = col.GetB() * (1. / 255); 
    float a = col.GetA() * (1. / 255);
    glfwMakeContextCurrent(this->m_window);
    glClearColor(r, g, b, a);
}

void WindowManager::Add(Renderable& obj) {
    this->m_renderObjects.push_back(&obj);
}

// Callbacks

void WindowManager::SetCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(this->m_window, callback);
}

void WindowManager::SetCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->m_window, callback);
}

void WindowManager::SetCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->m_window, callback);
}

void WindowManager::SetCallback(GLFWcharfun callback) {
    glfwSetCharCallback(this->m_window, callback);
}

void WindowManager::SetCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->m_window, callback);
}

void WindowManager::SetCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(this->m_window, callback);
}

// ----- Read -----

bool WindowManager::ShouldClose() {
    return glfwWindowShouldClose(this->m_window);
}

void WindowManager::Poll() {
    glfwMakeContextCurrent(this->m_window);
    glfwPollEvents();
}

void WindowManager::Render() {
    // Check if render operation is needed
    if (this->ShouldClose()) {
        return;
    }

    // Check time since last frame
    if (!this->HasFrameTimePassed()) {
        return;
    }
    
    // Set background
    glfwMakeContextCurrent(this->m_window);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(this->m_window);

    // Render objects
    for (Renderable* obj : this->m_renderObjects) {
        obj->Render();
    }
}

bool WindowManager::HasFrameTimePassed() {
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

