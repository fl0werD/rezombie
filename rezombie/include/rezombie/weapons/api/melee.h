#pragma once

#include "rezombie/core/api/amxx_feature.h"

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

        auto registerNatives() const -> void override;
    };

    inline AmxxMelee amxxMelee;
}
