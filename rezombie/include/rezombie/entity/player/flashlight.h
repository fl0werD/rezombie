#pragma once

#include "rezombie/core/object.h"
#include <utility>

namespace rz
{
    class Flashlight {
        std::string color_{};
        int size_{};
        int distance_ = 2048;
        float drainTime_{};
        float chargeTime_{};

      public:
        Flashlight(
            std::string color,
            int size,
            float drainTime,
            float chargeTime
        ) : color_(std::move(color)),
            size_(size),
            drainTime_(drainTime),
            chargeTime_(chargeTime) {}

        auto getColor() const -> const std::string& { return color_; }

        auto setColor(const std::string& color) -> void { color_ = color; }

        auto getSize() const -> int { return size_; }

        auto setSize(int size) -> void { size_ = size; }

        auto getDistance() const -> int { return distance_; }

        auto setDistance(int distance) -> void { distance_ = distance; }

        auto getDrainTime() const -> float { return drainTime_; }

        auto setDrainTime(float drainTime) -> void { drainTime_ = drainTime; }

        auto getChargeTime() const -> float { return chargeTime_; }

        auto setChargeTime(float chargeTime) -> void { chargeTime_ = chargeTime; }
    };
}
