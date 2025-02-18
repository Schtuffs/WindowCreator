#include "WindowManager.h"

// Holds if a window has been created
bool WindowManager::s_initialized = false;
unsigned short WindowManager::s_totalWindows = 0;

// ----- OpenGL ----- Functions -----

// Initialized OpenGL
void WindowManager::init() {
    if (WindowManager::s_initialized) {
        return;
    }
    // Init glfw
    if (!glfwInit()) {
        std::cout << "Could not initialize glfw..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // GLFW v3.3, CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tracks OpenGL initialized
    WindowManager::s_initialized = true;
}

// Closes OpenGL
void WindowManager::close() {
    glfwTerminate();
}



// ----- Creation ----- Destruction -----

WindowManager::WindowManager(const std::string& title) {
    // Initialized glfw
    WindowManager::init();

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
    s_totalWindows++;
}

bool WindowManager::isCreated() {
    return this->m_created;
}

WindowManager::~WindowManager() {
    // Tracks window removal
    WindowManager::s_totalWindows--;

    // Free all alocated memory
    if (this->m_window != nullptr) {
        glfwDestroyWindow(this->m_window);
    }

    // If no windows left, close OpenGL and marks as uninitialized
    if (WindowManager::s_totalWindows == 0) {
        WindowManager::close();
        WindowManager::s_initialized = false;
    }
}



// ----- Setters -----

void WindowManager::setBackground(Colour col) {
    float r = col.getR() * (1. / COLOUR_MAX);
    float g = col.getG() * (1. / COLOUR_MAX);
    float b = col.getB() * (1. / COLOUR_MAX); 
    float a = col.getA() * (1. / COLOUR_MAX);
    glfwMakeContextCurrent(this->m_window);
    glClearColor(r, g, b, a);
}

void WindowManager::add(Renderable& obj) {
    this->m_renderObjects.push_back(&obj);
}



// ----- Callbacks -----

void WindowManager::setCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(this->m_window, callback);
}

void WindowManager::setCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->m_window, callback);
}

void WindowManager::setCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->m_window, callback);
}

void WindowManager::setCallback(GLFWcharfun callback) {
    glfwSetCharCallback(this->m_window, callback);
}

void WindowManager::setCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->m_window, callback);
}

void WindowManager::setCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(this->m_window, callback);
}



// ----- Read -----

bool WindowManager::shouldClose() {
    return glfwWindowShouldClose(this->m_window);
}

void WindowManager::poll() {
    glfwMakeContextCurrent(this->m_window);
    glfwPollEvents();
}

void WindowManager::render() {
    // Check if render operation is needed
    if (this->shouldClose()) {
        return;
    }

    // Check time since last frame
    if (!this->hasFrameTimePassed()) {
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

bool WindowManager::hasFrameTimePassed() {
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

