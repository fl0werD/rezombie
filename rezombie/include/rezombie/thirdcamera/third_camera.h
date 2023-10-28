#pragma once

#include "rezombie/main/util.h"
#include <cssdk/public/regamedll.h>
#include <string>
#include <vector>
#include <array>

namespace rz
{
    using namespace cssdk;

    class ThirdCamera {
        Edict* camera_{};
        float distance_ = 96;

      public:
        auto createEntity() -> void;

        auto getCamera() -> Edict* { return camera_; }

        auto setCamera(Edict* camera) { camera_ = camera; }

        auto getDistance() const -> float { return distance_; }

        auto setDistance(float distance) -> void { distance_ = distance; }
    };

    inline ThirdCamera ThirdCamera;
}
