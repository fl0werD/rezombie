#pragma once

#include "rezombie/core/amxx_feature.h"
#include <array>

namespace rz
{
    enum class WeaponVars : int {
        Handle,
        Name,
        ViewModel,
        PlayerModel,
        PlayerModelBody,
        WorldModel,
        WorldModelBody,
        Hud,
        MaxClip,
        MaxAmmo,
        InventorySlot,
        Weight,
        Flags,
        BaseAccuracy,
        CrosshairSize,
        ForwardDeploy,
        ForwardHolster,
        ForwardMaxSpeed,
        ForwardPrimaryAttack,
        ForwardSecondaryAttack,
        ForwardReload,
        ForwardIdle,
        ForwardFireRemaining,
    };

    class AmxxWeapon : public AmxxFeature<> {
      public:
        AmxxWeapon() : AmxxFeature("weapon") {}

        auto RegisterNatives() -> void override;
    };

    inline AmxxWeapon amxxWeapon;
}
