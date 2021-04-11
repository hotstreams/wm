#include <wm/window.hpp>

#include <wm/window_manager.hpp>

#include <stdexcept>

using namespace wm;

wm::Window::Window(std::string_view name, wm::size _size, WindowManager& _manager)
    : size {_size}, manager {_manager} {
    GLint opengl_attributes[] = { GLX_RGBA, None };
    xvisual = glXChooseVisual(manager.getDisplay(), 0, opengl_attributes);

    if (!xvisual) {
        std::runtime_error("No specified visual info found");
    }

    colormap = XCreateColormap(manager.getDisplay(), manager.getDisplay().getRootWindow(), xvisual->visual, AllocNone);

    XSetWindowAttributes attributes;
    attributes.colormap = colormap;
    attributes.event_mask =  ExposureMask |
                             ButtonPressMask |
                             ButtonReleaseMask |
                             PointerMotionMask |
                             StructureNotifyMask;

    window = XCreateWindow(manager.getDisplay(),
                           manager.getDisplay().getRootWindow(),
                           0, 0,
                           size.x, size.y,
                           0,
                           xvisual->depth,
                           InputOutput,
                           xvisual->visual,
                           CWColormap | CWEventMask,
                           &attributes);

    auto atom = XInternAtom(manager.getDisplay(), "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(manager.getDisplay(), window, &atom, 1);

    clearWindow();

    mapWindow();

    storeName(name);

    flush();

    context = glXCreateContext(manager.getDisplay(), xvisual, nullptr, GL_TRUE);

    if (!context) {
        throw std::runtime_error("Failed to create glX context");
    }

    makeContextCurrent();

    manager.add(*this);
}

void wm::Window::setMotionCallback(std::function<void(int, int)> callback) noexcept {
    motion_callback = std::move(callback);
}

void wm::Window::setButtonCallback(std::function<void(Button, ButtonEvent)> callback) noexcept {
    button_callback = std::move(callback);
}

void wm::Window::setSizeCallback(std::function<void(uint32_t, uint32_t)> callback) noexcept {
    size_callback = std::move(callback);
}

wm::Window::~Window() {
    manager.remove(*this);

    glXDestroyContext(manager.getDisplay(), context);

    XFree(xvisual);
    XFreeColormap(manager.getDisplay(), colormap);
    XDestroyWindow(manager.getDisplay(), window);

    flush();
}

void wm::Window::makeContextCurrent() noexcept {
    glXMakeCurrent(manager.getDisplay(), window, context);
}

void wm::Window::flush() noexcept {
    XFlush(manager.getDisplay());
}

void wm::Window::storeName(std::string_view name) noexcept {
    XStoreName(manager.getDisplay(), window, name.data());
}

void wm::Window::mapWindow() noexcept {
    XMapWindow(manager.getDisplay(), window);
}

void wm::Window::clearWindow() noexcept {
    XClearWindow(manager.getDisplay(), window);
}
