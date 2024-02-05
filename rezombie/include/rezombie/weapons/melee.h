#pragma once

#include "rezombie/entity/weapons/base_weapon.h"

namespace rz
{
    enum KnifeAnimations {
        KNIFE_IDLE,
        KNIFE_ATTACK1_HIT,
        KNIFE_ATTACK2_HIT,
        KNIFE_DRAW,
        KNIFE_STAB_HIT,
        KNIFE_STAB_MISS,
        KNIFE_MIDDLE_ATTACK1_HIT,
        KNIFE_MIDDLE_ATTACK2_HIT,
    };

    class MeleeVirtuals : public PlayerWeaponBase {
        static VirtualHook addToPlayer;
        static VirtualHook deploy;
        static VirtualHook maxSpeed;
        static VirtualHook primaryAttack;
        static VirtualHook secondaryAttack;
        static VirtualHook idle;
        static VirtualHook create;
        static VirtualHook destroy;

      protected:
        auto MeleeAddToPlayer(PlayerBase* basePlayer) -> qboolean;
        auto MeleeDeploy() -> qboolean;
        auto MeleeMaxSpeed() -> float;
        auto MeleePrimaryAttack() -> void;
        auto MeleeSecondaryAttack() -> void;
        auto MeleeIdle() -> void;
        auto MeleeCreate() -> void;
        auto MeleeDestroy() -> void;
    };
}
