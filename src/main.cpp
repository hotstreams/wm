#include <wm/window_manager.hpp>
#include <wm/window.hpp>

class Application {
private:
    wm::WindowManager manager;
    wm::Window window;

    wm::posf triangle_position {};
    bool triangle {};
    bool moving {};

    static constexpr auto window_title = "Triangle demo";
    static constexpr auto window_size = wm::size {600, 600};
public:
    Application() : window {window_title, window_size, manager} {
        window.setMotionCallback([&] (int x, int y) {
            if (moving) {
                triangle_position = wm::convertSStoNDC({x, y}, window.getSize());
            }
        });

        window.setButtonCallback([&] (wm::Button button, wm::ButtonEvent event) {
            if (button == wm::Button::Left && event == wm::ButtonEvent::Pressed) {
                triangle_position = wm::convertSStoNDC(manager.getCursorPosition(window), window.getSize());
                triangle = true;
                moving = true;
            }

            if (button == wm::Button::Left && event == wm::ButtonEvent::Released) {
                moving = false;
            }
        });
    }

    void draw() {
        // constexpr auto background_color = 0x25854b;
        glClearColor(0.145098039216f, 0.521568627451f, 0.294117647059f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, window.getSize().x, window.getSize().y);

        if (triangle) {
            constexpr auto triangle_size = 0.3f;
            // fixed-function pipeline, love it, supported everywhere :)
            glBegin(GL_TRIANGLES);
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(triangle_position.x, triangle_position.y, 0.0f);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(triangle_position.x + triangle_size / 2.0f, triangle_position.y - triangle_size, 0.0f);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(triangle_position.x - triangle_size / 2.0f, triangle_position.y - triangle_size, 0.0f);
            glEnd();
        }

        manager.swapBuffers(window);
    }

    void run() {
        while (!window.shouldClose()) {
            draw();
            manager.pollEvents();
        }
    }
};

int main() {
    Application app;
    app.run();
    return 0;
}
