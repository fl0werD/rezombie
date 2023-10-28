#pragma once

#include <cssdk/common/vector.h>

namespace rz
{
    using namespace cssdk;

    class MapCameraVars {
        bool isEnabled_{};
        Vector origin_{};
        Vector angles_{};
        int cameraId_{};
        float nextCameraTime_{};

      public:
        auto isEnabled() const -> bool { return isEnabled_; }

        auto setEnabled(bool isEnabled) -> void { isEnabled_ = isEnabled; }

        auto getOrigin() const -> const Vector& { return origin_; }

        auto setOrigin(const Vector& origin) -> void { origin_ = origin; }

        auto getAngles() const -> const Vector& { return angles_; }

        auto setAngles(const Vector& angles) -> void { angles_ = angles; }

        auto getCamera() const -> int { return cameraId_; }

        auto setCamera(int cameraId) -> void { cameraId_ = cameraId; }

        auto getNextCameraTime() const -> float { return nextCameraTime_; }

        auto setNextCameraTime(float nextTime) -> void { nextCameraTime_ = nextTime; }
    };
}
