#pragma once

#include <wm/display.hpp>
#include <wm/util.hpp>

#include <unordered_map>

namespace wm {
    class Window;

    class WindowManager {
    private:
        std::unordered_map<::Window, Window*> windows;
        Display display;

        Atom wm_delete_window_atom;
    public:
        WindowManager();
        ~WindowManager() = default;

        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

        WindowManager(WindowManager&&) = delete;
        WindowManager& operator=(WindowManager&&) = delete;

        void add(Window& window);
        void remove(Window& window);

        void pollEvents();
        void swapBuffers(Window& window);
        pos getCursorPosition(Window& window);

        auto& getDisplay() { return display; }
    };
}
