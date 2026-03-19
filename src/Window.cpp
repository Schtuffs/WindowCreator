#include "Window.h"

#include <chrono>
#include <print>
#include <thread>

#include "WindowManager.h"

using namespace std::chrono_literals;

// Holds values for opening/closing OpenGL
static bool s_forceClose = false;
// Holds a specified main window
static GLFWwindow* s_mainWindow = nullptr;
// Ensure the await is on main thread
static std::thread::id s_mainThread = std::this_thread::get_id();
// Holds pointer to all windows
static std::vector<Window*> s_windows;

// ----- Creation ----- Destruction -----

Window::Window(const std::string& title) {
    // Add window to count
    WindowManager::add();
    m_width = WINDOW_SIZE_X;
    m_height = WINDOW_SIZE_Y;

    // Create the window
    this->m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);
    if (this->m_window == nullptr) {
        std::println(stderr, "ERROR: Could not create window.");
        this->m_created = false;

        // Remove failed window
        WindowManager::remove();
        return;
    }

    this->m_created = true;
    glfwMakeContextCurrent(this->m_window);
    if (s_mainWindow == nullptr) {
        setMain();
    }
    
    // Glad initialization
    gladLoadGL();
    glViewport(0, 0, m_width, m_height);

    // Setting blendmode so .png have blank backgrounds
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable vsync
    glfwSwapInterval(0);
    
    // Other variables
    this->m_prevTime = std::chrono::nanoseconds(std::chrono::system_clock::now().time_since_epoch()).count();
    this->m_frameTime = 0;
    this->m_sizeChanged = false;
    this->m_closed = false;
}

bool Window::isCreated() {
    return this->m_created;
}

Window::~Window() {
    this->close();
}

void Window::close() {
    if (this->m_window != nullptr) {
        glfwDestroyWindow(this->m_window);
        this->m_window = nullptr;
        WindowManager::remove();
    }
}

void Window::await(const std::function<void(void)>& lambda) {
    // Force main thread only
    if (std::this_thread::get_id() != s_mainThread) {
        std::println(stderr, "ERROR: Await must be called on the main thread.");
        return;
    }
    
    // Wait until windows are closed
    while (WindowManager::hasWindows()) {
        // Poll events then call the users lambda function
        glfwPollEvents();
        lambda();
        
        // This thread must close the windows
        for (size_t i = 0; i < s_windows.size(); i++) {
            if (s_windows[i]->m_closed) {
                // Close window, erase from list, and adjust the index for removed item
                s_windows[i]->close();
                s_windows.erase(s_windows.begin() + i);
                i--;
            }
        }
    }
}



// ----- Setters -----

Window& Window::setMain() {
    if (m_window == nullptr) {
        return *this;
    }

    s_mainWindow = this->m_window;
    return *this;
}    

Window& Window::setBackground(Colour col) {
    if (m_window == nullptr) {
        return *this;
    }

    float r = col.r * (1. / COLOUR_MAX);
    float g = col.g * (1. / COLOUR_MAX);
    float b = col.b * (1. / COLOUR_MAX); 
    float a = col.a * (1. / COLOUR_MAX);
    glfwMakeContextCurrent(this->m_window);
    glClearColor(r, g, b, a);

    return *this;
}    

Window& Window::setSize(int width, int height) {
    if (m_window == nullptr) {
        return *this;
    }

    // Change size variable
    m_width = width;
    m_height = height;
    
    // Get lock for allowing thread to update size
    m_sizeLock.lock();
    m_sizeChanged = true;
    m_sizeLock.unlock();
    
    return *this;
}    

Window& Window::setTitle(const std::string& title) {
    if (m_window == nullptr) {
        return *this;
    }
    
    glfwSetWindowTitle(m_window, title.c_str());
    return *this;
}

Window& Window::add(Renderable& obj) {
    this->m_renderObjects.push_back(&obj);
    return *this;
}

bool Window::remove(uint64_t index) {
    // TODO - add removal logic
    (void)index;
    return false;
}

bool Window::remove(Renderable& obj) {
    // TODO - add removal logic
    (void)obj;
    return false;
}



// ----- Callbacks -----

Window& Window::setCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWcharfun callback) {
    glfwSetCharCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->m_window, callback);
    return *this;
}

Window& Window::setCallback(GLFWwindowrefreshfun callback) {
    glfwSetWindowRefreshCallback(this->m_window, callback);
    return *this;
}



// ----- Update -----

void Window::run(uint64_t frames, bool upsMatchFps) {
    // Remove context to set window in thread
    glfwMakeContextCurrent(nullptr);
    if (frames == 0) {
        m_frameTime = 0;
    }
    else {
        m_frameTime = std::chrono::nanoseconds((std::chrono::nanoseconds(1s).count() / frames)).count();
    }

    // Add window to list for main thread to close
    s_windows.emplace_back(this);
    
    // Put onto thread
    std::thread t(&Window::showWindow, this, upsMatchFps);
    t.detach();
}

bool Window::hasFrameTimePassed(bool upsMatchFps) {
    // If no specified rendering fps
    if (this->m_frameTime == 0) {
        return true;
    }
    
    // Sleeps and return true so the Updates per Second match the Frames per Second
    if (upsMatchFps) {
        // TODO - fix this being slightly too slow (allows ~59 cycles per minute at 60ups, not close enough to 59.9)
        uint64_t current = std::chrono::nanoseconds(std::chrono::system_clock::now().time_since_epoch()).count();
        uint64_t delta = current - m_prevTime;
        delta -= m_frameTime;
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_frameTime - delta));

        m_prevTime = current;
        return true;
    }

    // UPS can be anything, so no sleep
    uint64_t current = std::chrono::nanoseconds(std::chrono::system_clock::now().time_since_epoch()).count();
    if (current > m_prevTime && m_frameTime <= (current - m_prevTime)) {
        // Updates the previous time based on current, previous, and frametime
        // current - previous = elapsed time
        // elapsed - frame = true delta
        // current - delta = adjusted time
        m_prevTime = current - ((current - m_prevTime) - m_frameTime);
        return true;
    }
    return false;
}

void Window::render(bool upsMatchFps) {
    // Check time since last frame
    if (!this->hasFrameTimePassed(upsMatchFps)) {
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

void Window::checkSizeChange() {
    // Prevent invalid read
    m_sizeLock.lock();
    
    if (m_sizeChanged) {
        // Size changes, update values
        glfwSetWindowSize(m_window, m_width, m_height);
        glViewport(0, 0, m_width, m_height);
        m_sizeChanged = false;
    }
    m_sizeLock.unlock();
}

void Window::checkChanges() {
    // Checks for size change
    checkSizeChange();
}

bool Window::shouldClose() {
    // If this is the main window, check if its closed
    if (s_mainWindow != nullptr && this->m_window == s_mainWindow) {
        s_forceClose = glfwWindowShouldClose(this->m_window);
        return s_forceClose;
    }
    
    return glfwWindowShouldClose(this->m_window);
}

void Window::showWindow(bool upsMatchFps) {
    std::println("Thread made on {}", std::this_thread::get_id());
    
    // Set window on this thread
    glfwMakeContextCurrent(this->m_window);
    
    // Loop while window is open
    while (!this->shouldClose() && !s_forceClose) {
        // Check for any changes to window
        this->checkChanges();

        // Render object
        this->render(upsMatchFps);
    }
    
    // Window is closed
    glfwMakeContextCurrent(nullptr);
    std::println("Thread ended on {}", std::this_thread::get_id());
    m_closed = true;
}

