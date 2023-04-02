#pragma once

#include "rezombie/weapons/base_weapon.h"
#include "rezombie/weapons/weapons.h"
#include <string>

namespace rz::weapon
{
    class Weapon : public BaseWeapon
    {
      private:
        // int speed; but need hook all weapons (or its member?)
        // String weaponList = "";

      public:
        Weapon(std::string handle, WeaponType weaponType) : BaseWeapon(std::move(handle), "weapon_ak47", weaponType)
        {
        }
    };
}
