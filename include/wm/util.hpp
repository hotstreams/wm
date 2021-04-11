#pragma once

#include <cstdint>

namespace wm {
    struct pos { int x; int y; };
    struct posf { float x; float y; };
    struct size { uint32_t x; uint32_t y; };

    // converts screen space coordinates to NDC
    inline posf convertSStoNDC(const pos& position, const size& size) {
        auto x = static_cast<float>(position.x) / static_cast<float>(size.x);
        auto y = static_cast<float>(position.y) / static_cast<float>(size.y);

        x = x * 2.0f - 1.0f;
        y = 1.0f - y * 2.0f;

        return { x, y };
    }
}