#pragma once

#include "rezombie/player/modules/nightvision.h"

namespace rz
{
    class NightVision {
        std::string color_{};
        int fogId_{};

      public:
        NightVision(
            std::string color,
            int fogId
        ) : color_(std::move(color)),
            fogId_(fogId) {}

        auto getColor() const -> const std::string& { return color_; }

        auto setColor(const std::string& color) -> void { color_ = color; }

        auto getFog() const -> int { return fogId_; }

        auto setFog(int fogId) -> void { fogId_ = fogId; }
    };
}
