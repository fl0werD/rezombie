#pragma once

#include "cssdk/public/regamedll.h"

namespace rz
{
    using namespace cssdk;

    class CameraData {
        Vector origin_;
        Vector angles_;

      public:
        CameraData(
            Vector origin,
            Vector angles
        ) : origin_(origin),
            angles_(angles) {}

        auto getOrigin() const -> const Vector& { return origin_; }

        auto getAngles() const -> const Vector& { return angles_; }
    };
}
