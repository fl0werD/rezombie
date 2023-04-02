#pragma once

#include "rezombie/core/amxx_feature.h"
#include <array>

namespace rz
{
    enum class PlayerPropsVars : int
    {
        Handle,
        Health,
        BaseHealth,
        Armor,
        Helmet,
        Speed,
        Gravity,
        Footsteps,
        WeaponsInteraction,
        VelModBody,
        VelModHead,
        Knockback,
        BloodColor,
        RenderMode,
        RenderAmount,
        RenderColor,
        RenderFx,
    };

    class AmxxPlayerProps : public AmxxFeature<>
    {
      public:
        AmxxPlayerProps() : AmxxFeature("player_props")
        {
        }

        auto RegisterNatives() -> void override;
    };

    inline AmxxPlayerProps amxxPlayerProps;
}
