#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include <string>
#include <unordered_map>

namespace rz
{
    class AmxxWeapon : public AmxxFeature<> {
      public:
        AmxxWeapon() : AmxxFeature("weapons") {}

        auto registerNatives() const -> void override;
    };

    inline AmxxWeapon WeaponApi;
}
