#pragma once

#include "rezombie/main/util.h"
#include "cssdk/public/regamedll.h"
#include <string>
#include <vector>
#include <array>

namespace rz
{
    using namespace cssdk;

    class WorldPreview {
        Edict* entity_{};
        float viewForwardDistance_ = 96;
        float buttonsRotateSpeed_ = 2;

      public:
        auto createEntity() -> void;

        auto getEntity() -> Edict* { return entity_; }

        auto setEntity(Edict* entity) { entity_ = entity; }

        auto getViewForwardDistance() const -> float { return viewForwardDistance_; }

        auto setViewForwardDistance(float viewForwardDistance) -> void { viewForwardDistance_ = viewForwardDistance; }
    };

    inline WorldPreview WorldPreview;
}
