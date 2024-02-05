#pragma once

#include <cssdk/common/vector.h>

namespace rz
{
    using namespace cssdk;

    class JoinPreviewVars {
        bool isEnabled_{};
        Vector origin_{};
        Vector angles_{};
        std::array<int, toInt(JoinPreviewType::MAX_TYPES)> modelIds_{};

      public:
        auto isEnabled() const -> bool { return isEnabled_; }

        auto setEnabled(bool isEnabled) -> void { isEnabled_ = isEnabled; }

        auto getOrigin() const -> const Vector& { return origin_; }

        auto setOrigin(const Vector& origin) -> void { origin_ = origin; }

        auto getAngles() const -> const Vector& { return angles_; }

        auto setAngles(const Vector& angles) -> void { angles_ = angles; }

        auto getModel(JoinPreviewType previewType) const -> int { return modelIds_[toInt(previewType)]; }

        auto setModel(JoinPreviewType previewType, int modelId) -> void { modelIds_[toInt(previewType)] = modelId; }
    };
}
