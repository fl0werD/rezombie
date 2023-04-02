#pragma once

namespace rz
{
    enum class WeaponVars : int
    {
        Handle,
        Name,
        ViewModel,
        PlayerModel,
        WorldModel,
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

    auto RegisterWeaponNatives() -> void;
}
