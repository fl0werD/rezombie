#pragma once

namespace rz
{
    class Color {
        unsigned char red_;
        unsigned char green_;
        unsigned char blue_;
        unsigned char alpha_;

      public:
        Color(
            unsigned char red = 0,
            unsigned char green = 0,
            unsigned char blue = 0,
            unsigned char alpha = 0
        ) : red_(red),
            green_(green),
            blue_(blue),
            alpha_(alpha) {}

        auto getRed() const -> unsigned char { return red_; }

        auto getGreen() const -> unsigned char { return green_; }

        auto getBlue() const -> unsigned char { return blue_; }

        auto getAlpha() const -> unsigned char { return alpha_; }
    };
}
