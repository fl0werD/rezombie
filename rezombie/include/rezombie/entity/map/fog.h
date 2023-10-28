#pragma once

#include <string>

namespace rz
{
    class Fog {
        std::string color_{};
        int densityPercentage_{};

      public:
        Fog(
            std::string color,
            int densityPercentage
        ) : color_(std::move(color)),
            densityPercentage_(densityPercentage) {}

        auto getColor() -> const std::string& { return color_; }

        auto setColor(const std::string& color) -> void { color_ = color; }

        auto getDensityPercentage() const -> int { return densityPercentage_; }

        auto setDensityPercentage(int densityPercentage) -> void { densityPercentage_ = densityPercentage; }
    };
}
