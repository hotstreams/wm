#pragma once

#include <X11/Xlib.h>

namespace wm {
    class Display {
    private:
        ::Display* display;
        ::Window root_window;
    public:
        Display();
        ~Display();

        Display(const Display&) = delete;
        Display& operator=(const Display&) = delete;

        Display(Display&&) = delete;
        Display& operator=(Display&&) = delete;

        [[nodiscard]] auto* getX11Display() const noexcept { return display; }
        [[nodiscard]] auto getRootWindow() const noexcept { return root_window; }

        operator ::Display*() const noexcept { return display; }
    };
}
