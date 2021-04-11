#include <wm/display.hpp>

#include <stdexcept>

using namespace wm;

wm::Display::Display() {
    display = XOpenDisplay(nullptr);

    if (!display) {
        throw std::runtime_error("Failed to open display connection");
    }

    root_window = DefaultRootWindow(display);
}

wm::Display::~Display() {
    XCloseDisplay(display);
}
