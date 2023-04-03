#pragma once

#include "rezombie/core/base_weapon.h"
#include <cssdk/dll/weapon_type.h>

namespace rz::weapon
{
    const std::string WEAPON_MELEE = "weapon_knife";

    class Melee : public BaseWeapon {
      private:
        int swingBaseDamage_ = 15;
        int swingDistance_ = 48;
        int stabBaseDamage_ = 65;
        int stabDistance_ = 32;
        // float damageMulti;
        // Array: Melee_Sounds[MAX_MELEE_SOUNDS];

      public:
        explicit Melee(std::string handle) : BaseWeapon(
            std::move(handle),
            WEAPON_MELEE,
            WeaponType::Melee
        ) {}
    };

    class MeleeVirtuals : public PlayerWeaponBase {
      private:
        static VirtualHook deploy;
        static VirtualHook primaryAttack;
        static VirtualHook secondaryAttack;

      protected:
        auto MeleeDeploy() -> qboolean;

        auto MeleePrimaryAttack() -> void;

        auto MeleeSecondaryAttack() -> void;
    };
}
