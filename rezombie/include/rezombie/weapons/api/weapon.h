#pragma once

#include "rezombie/core/api/amxx_feature.h"

namespace rz
{
    class AmxxWeapon : public AmxxFeature<> {
      public:
        AmxxWeapon() : AmxxFeature("weapons") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxWeapon WeaponApi;
}
