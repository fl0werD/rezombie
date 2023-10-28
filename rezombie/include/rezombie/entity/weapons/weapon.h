#pragma once

#include "rezombie/entity/weapons/base_weapon.h"
#include "rezombie/weapons/weapons.h"
#include <string>

namespace rz
{
    class Weapon : public BaseWeapon {
        float baseAccuracy_ = 0.f;
        CrosshairSize crosshairSize_ = CrosshairSize::None;
        int fireRemainingForward_ = -1;

      public:
        Weapon(
            std::string handle,
            WeaponType weaponType
        ) : BaseWeapon(std::move(handle), WEAPON_PLACEHOLDER, weaponType) {}

        auto getBaseAccuracy() const { return baseAccuracy_; }

        auto setBaseAccuracy(float baseAccuracy) { baseAccuracy_ = baseAccuracy; }

        auto getCrosshairSize() const { return crosshairSize_; }

        auto setCrosshairSize(CrosshairSize crosshairSize) { crosshairSize_ = crosshairSize; }

        auto getFireRemainingForward() const { return fireRemainingForward_; }

        auto setFireRemainingForward(int fireRemainingForward) { fireRemainingForward_ = fireRemainingForward; }

        auto executeFireRemaining(int weapon, int player, int clip, int shotsFired) const -> bool {
            if (getFireRemainingForward() == -1) {
                return false;
            }
            ExecuteForward(getFireRemainingForward(), weapon, player, clip, shotsFired);
            return true;
        }
    };
}
