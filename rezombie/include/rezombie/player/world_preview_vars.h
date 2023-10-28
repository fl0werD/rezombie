#pragma once

#include <cssdk/common/vector.h>

namespace rz
{
    using namespace cssdk;

    class WorldPreviewVars {
        bool isEnabled_{};
        Vector origin_{};
        Vector angles_{};
        int modelId_{};

      public:
        auto isEnabled() const -> bool { return isEnabled_; }

        auto setEnabled(bool isEnabled) -> void { isEnabled_ = isEnabled; }

        auto getOrigin() const -> const Vector& { return origin_; }

        auto setOrigin(const Vector& origin) -> void { origin_ = origin; }

        auto getAngles() const -> const Vector& { return angles_; }

        auto setAngles(const Vector& angles) -> void { angles_ = angles; }

        auto getModel() const -> int { return modelId_; }

        auto setModel(int modelId) -> void { modelId_ = modelId; }
    };
}
