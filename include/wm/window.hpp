#pragma once

#include <wm/event.hpp>
#include <wm/util.hpp>

#include <string_view>
#include <functional>

#include <GL/glx.h>

namespace wm {
    class WindowManager;

    class Window {
    private:
        ::Window window {};
        wm::size size {};
        bool should_close {};

        std::function<void(int x, int y)> motion_callback;
        std::function<void(Button, ButtonEvent)> button_callback;
        std::function<void(uint32_t w, uint32_t h)> size_callback;

        XVisualInfo* xvisual;
        Colormap colormap;
        GLXContext context;

        friend class WindowManager;
        WindowManager& manager;
    public:
        Window(std::string_view name, wm::size size, WindowManager& manager);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        void setButtonCallback(std::function<void(Button, ButtonEvent)> callback) noexcept;
        void setMotionCallback(std::function<void(int, int)> callback) noexcept;
        void setSizeCallback(std::function<void(uint32_t, uint32_t)> callback) noexcept;

        void flush() noexcept;
        void mapWindow() noexcept;
        void clearWindow() noexcept;
        void makeContextCurrent() noexcept;
        void storeName(std::string_view name) noexcept;

        [[nodiscard]] auto getX11Window() const noexcept { return window; }
        [[nodiscard]] auto shouldClose() const noexcept { return should_close; }
        [[nodiscard]] auto getSize() const noexcept { return size; }
    };
}
