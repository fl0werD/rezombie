#pragma once

#include "rezombie/core/amxx_feature.h"
#include <array>

namespace rz
{
    enum class PlayerModelHeader : int {
        Path,
        IsDefaultHitboxes,
        Body,
        ModelIndex,
    };

    class AmxxPlayerModel : public AmxxFeature<> {
      public:
        AmxxPlayerModel() : AmxxFeature("player_model") {}

        auto RegisterNatives() -> void override;
    };

    inline AmxxPlayerModel amxxPlayerModel;
}
