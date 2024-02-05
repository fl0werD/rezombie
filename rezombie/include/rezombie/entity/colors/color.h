#pragma once

#include <algorithm>

namespace rz
{
    class Color {
        uint8_t red_;
        uint8_t green_;
        uint8_t blue_;
        uint8_t alpha_;

        public:
            Color(
                const uint8_t red = 0,
                const uint8_t green = 0,
                const uint8_t blue = 0,
                const float alpha = 1.f
            ) : red_(red),
                green_(green),
                blue_(blue) {
                alpha_ = std::clamp(static_cast<uint8_t>(alpha * UINT8_MAX), static_cast<uint8_t>(0), UINT8_MAX);
            }

            auto getRed() const -> uint8_t { return red_; }

            auto getGreen() const -> uint8_t { return green_; }

            auto getBlue() const -> uint8_t { return blue_; }

            auto getAlpha() const -> uint8_t { return alpha_; }
    };
}
