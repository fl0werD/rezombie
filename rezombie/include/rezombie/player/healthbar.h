#pragma once

#include "rezombie/main/util.h"
#include "cssdk/public/regamedll.h"
#include <string>
#include <vector>
#include <array>

namespace rz
{
    using namespace cssdk;

    class HealthBar {
        Edict* entities_[MAX_CLIENTS]{};

      public:
        auto createEntity() -> void;

        auto precache() -> void;

        auto getEntity(int id) -> Edict* { return entities_[id]; }

        auto setEntity(int id, Edict* entity) { entities_[id] = entity; }
    };

    inline HealthBar HealthBar;
}
