#pragma once

namespace rz
{
    class FlashlightVars {
        int id_ = 0;
        bool isEnabled_ = false;
        int battery_ = 100;
        float time_ = 0.f;
        float nextDynamicUpdate_ = 0.f;

      public:
        auto getId() const -> int { return id_; }

        auto setId(int flashlightId) -> void { id_ = flashlightId; }

        auto isEnabled() const -> bool { return isEnabled_; }

        auto setEnabled(bool isEnabled) -> void { isEnabled_ = isEnabled; }

        auto getBattery() const -> int { return battery_; }

        auto setBattery(int battery) -> void { battery_ = battery; }

        auto getTime() const -> float { return time_; }

        auto setTime(float time) -> void { time_ = time; }

        auto getNextDynamicUpdate() const -> float { return nextDynamicUpdate_; }

        auto setNextDynamicUpdate(float time) -> void { nextDynamicUpdate_ = time; }
    };
}
