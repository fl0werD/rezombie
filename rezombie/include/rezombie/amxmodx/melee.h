#pragma once

#include "rezombie/core/amxx_feature.h"
#include <array>

namespace rz
{
    enum class MeleeVars : int {
        Handle,
        ViewModel,
        PlayerModel,
        WorldModel,
        WeaponList,
        Name,
    };

    class AmxxMelee : public AmxxFeature<> {
      public:
        AmxxMelee() : AmxxFeature("melee") {}

        auto RegisterNatives() -> void override;
    };

    inline AmxxMelee amxxMelee;
}
