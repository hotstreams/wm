#include <wm/window_manager.hpp>

#include <wm/window.hpp>

using namespace wm;

WindowManager::WindowManager() {
    wm_delete_window_atom = XInternAtom(display.getX11Display(), "WM_DELETE_WINDOW", false);
}

void WindowManager::add(Window& window) {
    windows.emplace(window.getX11Window(), &window);
}

void WindowManager::remove(Window& window) {
    windows.erase(window.getX11Window());
}

void WindowManager::pollEvents() {
    while (XPending(display.getX11Display())) {
        XEvent event;
        XNextEvent(display.getX11Display(), &event);

        switch (event.type) {
            case ButtonPress: {
                if (auto &callback = windows[event.xmotion.window]->button_callback; callback) {
                    if (event.xbutton.button == Button1) {
                        callback(Button::Left, ButtonEvent::Pressed);
                    }
                    if (event.xbutton.button == Button2) {
                        callback(Button::Middle, ButtonEvent::Pressed);
                    }
                    if (event.xbutton.button == Button3) {
                        callback(Button::Right, ButtonEvent::Pressed);
                    }
                }
                break;
            }
            case ButtonRelease: {
                if (auto &callback = windows[event.xmotion.window]->button_callback; callback) {
                    if (event.xbutton.button == Button1) {
                        callback(Button::Left, ButtonEvent::Released);
                    }
                    if (event.xbutton.button == Button2) {
                        callback(Button::Middle, ButtonEvent::Released);
                    }
                    if (event.xbutton.button == Button3) {
                        callback(Button::Right, ButtonEvent::Released);
                    }
                }
                break;
            }
            case MotionNotify: {
                if (auto &callback = windows[event.xmotion.window]->motion_callback; callback) {
                    callback(event.xmotion.x, event.xmotion.y);
                }
                break;
            }
            case ClientMessage: {
                const auto atom = event.xclient.data.l[0];
                if (atom == wm_delete_window_atom) {
                    windows[event.xclient.window]->should_close = true;
                }
                break;
            }
            case ConfigureNotify: {
                auto* window = windows[event.xconfigure.window];

                if (window->size.x != event.xconfigure.width || window->size.y != event.xconfigure.height) {
                    window->size.x = event.xconfigure.width;
                    window->size.y = event.xconfigure.height;

                    if (window->size_callback) {
                        window->size_callback(window->size.x, window->size.y);
                    }
                }

                break;
            }
            default:
                break;
        }
    }
}

void WindowManager::swapBuffers(wm::Window &window) {
    glXSwapBuffers(display.getX11Display(), window.getX11Window());
}

pos WindowManager::getCursorPosition(Window& window) {
    ::Window root_return, child_return;
    int root_x_return, root_y_return;
    unsigned int mask_return;
    int x;
    int y;

    XQueryPointer(display.getX11Display(), window.getX11Window(),
                  &root_return, &child_return,
                  &root_x_return, &root_y_return,
                  &x, &y,
                  &mask_return);

    return { x, y };
}

//void WindowManager::setSwapInterval(Window& window, int interval) {
//    actually, we have to check extension for this call
//    auto swapInterval = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress(reinterpret_cast<const GLubyte *>("glXSwapIntervalEXT"));
//    swapInterval(display.getX11Display(), window.getX11Window(), interval);
//}
