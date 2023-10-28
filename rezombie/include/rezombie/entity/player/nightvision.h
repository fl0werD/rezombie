#pragma once

#include <utility>

#include "rezombie/core/object.h"
#include "rezombie/player/modules/nightvision.h"

namespace rz
{
    class NightVision {
        std::string color_{};
        int alphaPercentage_{};
        int fogId_{};

      public:
        NightVision(
            std::string color,
            int alphaPercentage,
            int fogId
        ) : color_(std::move(color)),
            alphaPercentage_(alphaPercentage),
            fogId_(fogId) {}

        auto getColor() const -> const std::string& { return color_; }

        auto setColor(const std::string& color) -> void { color_ = color; }

        auto getAlphaPercentage() const -> int { return alphaPercentage_; }

        auto setAlphaPercentage(int alphaPercentage) -> void { alphaPercentage_ = alphaPercentage; }

        auto getFog() const -> int { return fogId_; }

        auto setFog(int fogId) -> void { fogId_ = fogId; }
    };
}
